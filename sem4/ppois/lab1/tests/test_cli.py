from conference.cli import run_cli
from conference.services import ConferenceService


def test_cli_create_and_show_conference(capsys) -> None:
    service = ConferenceService()

    create_code = run_cli(
        [
            "create-conference",
            "--name",
            "Research Summit 2026",
            "--start-date",
            "2026-04-10",
            "--end-date",
            "2026-04-12",
        ],
        service=service,
    )
    assert create_code == 0

    show_code = run_cli(["show-conference"], service=service)
    assert show_code == 0
    output = capsys.readouterr().out
    assert "Research Summit 2026" in output
    assert "Участники: 0" in output


def test_cli_fails_for_invalid_command_arguments() -> None:
    code = run_cli(
        [
            "create-conference",
            "--name",
            "Bad Date Case",
            "--start-date",
            "2026-04-99",
            "--end-date",
            "2026-04-12",
        ]
    )
    assert code != 0


def test_cli_rejects_scheduling_unknown_submission(capsys) -> None:
    service = ConferenceService()
    run_cli(
        [
            "create-conference",
            "--name",
            "Ops Forum 2026",
            "--start-date",
            "2026-07-01",
            "--end-date",
            "2026-07-02",
        ],
        service=service,
    )
    run_cli(
        [
            "add-session",
            "--session-id",
            "s-ops-01",
            "--track",
            "Operations",
            "--room",
            "D-105",
            "--start",
            "2026-07-01T10:00",
            "--end",
            "2026-07-01T11:00",
        ],
        service=service,
    )

    code = run_cli(
        [
            "assign-paper",
            "--session-id",
            "s-ops-01",
            "--submission-id",
            "paper-unknown",
        ],
        service=service,
    )
    assert code == 1
    output = capsys.readouterr().out
    assert "Ошибка:" in output


def test_cli_show_schedule_when_empty(capsys) -> None:
    service = ConferenceService()
    run_cli(
        [
            "create-conference",
            "--name",
            "Quality Engineering Days",
            "--start-date",
            "2026-03-15",
            "--end-date",
            "2026-03-16",
        ],
        service=service,
    )
    code = run_cli(["show-schedule"], service=service)
    assert code == 0
    output = capsys.readouterr().out
    assert "Расписание пустое." in output


def test_interactive_menu_create_show_and_exit(monkeypatch, capsys) -> None:
    service = ConferenceService()
    answers = iter(
        [
            "1",
            "Interactive Conf 2026",
            "2026-08-20",
            "2026-08-21",
            "2",
            "0",
        ]
    )
    monkeypatch.setattr("builtins.input", lambda _: next(answers))

    code = run_cli([], service=service)
    assert code == 0
    output = capsys.readouterr().out
    assert "Конференция 'Interactive Conf 2026' создана" in output
    assert "Конференция: Interactive Conf 2026" in output
    assert "Выход." in output


def test_interactive_menu_handles_unknown_option(monkeypatch, capsys) -> None:
    service = ConferenceService()
    answers = iter(["99", "0"])
    monkeypatch.setattr("builtins.input", lambda _: next(answers))

    code = run_cli([], service=service)
    assert code == 0
    output = capsys.readouterr().out
    assert "Неизвестный пункт меню. Повторите ввод." in output
