import argparse

import pytest

from conference.cli import (
    _execute_command,
    build_parser,
    parse_date,
    parse_datetime,
    run_cli,
)
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


def test_parser_help_and_usage_are_localized(capsys) -> None:
    parser = build_parser()
    with pytest.raises(SystemExit):
        parser.parse_args(["--help"])
    output = capsys.readouterr().out
    assert "использование:" in output
    assert "показать справку и выйти" in output


def test_parse_datetime_rejects_invalid_value() -> None:
    with pytest.raises(argparse.ArgumentTypeError):
        parse_datetime("2026-99-99T25:61")


def test_parse_date_accepts_valid_value() -> None:
    parsed = parse_date("2026-05-20")
    assert parsed.isoformat() == "2026-05-20"


def test_cli_full_happy_path_commands_cover_all_actions(capsys) -> None:
    service = ConferenceService()
    assert run_cli(
        [
            "create-conference",
            "--name",
            "All Commands Conference",
            "--start-date",
            "2026-11-10",
            "--end-date",
            "2026-11-11",
        ],
        service=service,
    ) == 0
    assert run_cli(
        [
            "register-participant",
            "--participant-id",
            "p-900",
            "--full-name",
            "Nikita Volkov",
            "--email",
            "nikita.volkov@example.com",
        ],
        service=service,
    ) == 0
    assert run_cli(
        [
            "submit-paper",
            "--submission-id",
            "paper-900",
            "--participant-id",
            "p-900",
            "--title",
            "Distributed Tracing",
            "--abstract",
            "Practical lessons from production systems",
        ],
        service=service,
    ) == 0
    assert run_cli(
        ["set-status", "--submission-id", "paper-900", "--status", "accepted"],
        service=service,
    ) == 0
    assert run_cli(
        [
            "add-session",
            "--session-id",
            "s-900",
            "--track",
            "Observability",
            "--room",
            "R-12",
            "--start",
            "2026-11-10T09:00",
            "--end",
            "2026-11-10T10:00",
        ],
        service=service,
    ) == 0
    assert run_cli(
        ["assign-paper", "--session-id", "s-900", "--submission-id", "paper-900"],
        service=service,
    ) == 0
    assert run_cli(["show-schedule"], service=service) == 0

    output = capsys.readouterr().out
    assert "Участник 'p-900' зарегистрирован." in output
    assert "Доклад 'paper-900' создан со статусом 'submitted'." in output
    assert "Статус доклада 'paper-900' теперь 'accepted'." in output
    assert "Сессия 's-900' создана." in output
    assert "Доклад 'paper-900' назначен в сессию 's-900'." in output
    assert "s-900: Observability | R-12" in output


def test_execute_command_handles_unknown_command_branch(capsys) -> None:
    service = ConferenceService()
    code = _execute_command(argparse.Namespace(command="unknown"), service)
    assert code == 1
    assert "неизвестная команда" in capsys.readouterr().out


def test_run_cli_returns_parser_exit_code_for_help() -> None:
    assert run_cli(["--help"]) == 0


def test_run_interactive_menu_register_and_submit_with_default_role(monkeypatch, capsys) -> None:
    service = ConferenceService()
    answers = iter(
        [
            "1",
            "Interactive Pipeline 2026",
            "2026-12-01",
            "2026-12-02",
            "3",
            "p-701",
            "Svetlana Romanova",
            "svetlana.romanova@example.com",
            "",
            "4",
            "paper-701",
            "p-701",
            "Reliable Messaging",
            "Engineering lessons",
            "0",
        ]
    )
    monkeypatch.setattr("builtins.input", lambda _: next(answers))

    code = run_cli([], service=service)
    assert code == 0
    output = capsys.readouterr().out
    assert "Участник 'p-701' зарегистрирован." in output
    assert "Доклад 'paper-701' создан со статусом 'submitted'." in output


def test_run_interactive_menu_set_status_and_assign(monkeypatch, capsys) -> None:
    service = ConferenceService()
    answers = iter(
        [
            "1",
            "Interactive Ops 2026",
            "2026-12-10",
            "2026-12-11",
            "3",
            "p-801",
            "Ivan Nikitin",
            "ivan.nikitin@example.com",
            "speaker",
            "4",
            "paper-801",
            "p-801",
            "Zero Downtime Deployments",
            "War stories and patterns",
            "5",
            "paper-801",
            "accepted",
            "6",
            "s-801",
            "Operations",
            "D-9",
            "2026-12-10T10:00",
            "2026-12-10T11:00",
            "7",
            "s-801",
            "paper-801",
            "8",
            "0",
        ]
    )
    monkeypatch.setattr("builtins.input", lambda _: next(answers))

    code = run_cli([], service=service)
    assert code == 0
    output = capsys.readouterr().out
    assert "Статус доклада 'paper-801' теперь 'accepted'." in output
    assert "Доклад 'paper-801' назначен в сессию 's-801'." in output
    assert "s-801: Operations | D-9" in output


def test_run_interactive_menu_handles_argument_type_error(monkeypatch, capsys) -> None:
    service = ConferenceService()
    answers = iter(["1", "Bad Menu Dates", "2026-14-40", "2026-12-11", "0"])
    monkeypatch.setattr("builtins.input", lambda _: next(answers))

    code = run_cli([], service=service)
    assert code == 0
    output = capsys.readouterr().out
    assert "Дата '2026-14-40' некорректна" in output


def test_run_interactive_menu_handles_keyboard_interrupt(monkeypatch, capsys) -> None:
    def raise_keyboard_interrupt(_: str) -> str:
        raise KeyboardInterrupt

    monkeypatch.setattr("builtins.input", raise_keyboard_interrupt)
    assert run_cli([]) == 0
    assert "Выход." in capsys.readouterr().out
