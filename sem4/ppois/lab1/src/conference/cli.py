import argparse
from datetime import date, datetime
from typing import Sequence

from conference.models import DomainError
from conference.services import ConferenceService


class RussianArgumentParser(argparse.ArgumentParser):
    def format_usage(self) -> str:
        return super().format_usage().replace("usage:", "использование:")

    def format_help(self) -> str:
        help_text = super().format_help()
        return (
            help_text.replace("usage:", "использование:")
            .replace("show this help message and exit", "показать справку и выйти")
        )

    def error(self, message: str) -> None:
        self.print_usage()
        raise SystemExit(f"Ошибка: {message}")


def parse_date(raw_value: str) -> date:
    try:
        return date.fromisoformat(raw_value)
    except ValueError as error:
        raise argparse.ArgumentTypeError(
            f"Дата '{raw_value}' некорректна. Используйте формат YYYY-MM-DD."
        ) from error


def parse_datetime(raw_value: str) -> datetime:
    try:
        return datetime.fromisoformat(raw_value)
    except ValueError as error:
        raise argparse.ArgumentTypeError(
            f"Дата и время '{raw_value}' некорректны. Используйте формат YYYY-MM-DDTHH:MM."
        ) from error


def build_parser() -> argparse.ArgumentParser:
    parser = RussianArgumentParser(
        prog="conference",
        description="Управление научной конференцией через CLI.",
    )
    parser._positionals.title = "позиционные аргументы"
    parser._optionals.title = "опции"
    subparsers = parser.add_subparsers(dest="command", title="команды")

    create = subparsers.add_parser("create-conference", help="Создать конференцию")
    create.add_argument("--name", required=True)
    create.add_argument("--start-date", required=True, type=parse_date)
    create.add_argument("--end-date", required=True, type=parse_date)

    subparsers.add_parser("show-conference", help="Показать конференцию")

    register = subparsers.add_parser("register-participant", help="Зарегистрировать участника")
    register.add_argument("--participant-id", required=True)
    register.add_argument("--full-name", required=True)
    register.add_argument("--email", required=True)
    register.add_argument("--role", default="attendee")

    submit = subparsers.add_parser("submit-paper", help="Подать доклад")
    submit.add_argument("--submission-id", required=True)
    submit.add_argument("--participant-id", required=True)
    submit.add_argument("--title", required=True)
    submit.add_argument("--abstract", required=True)

    status = subparsers.add_parser("set-status", help="Изменить статус доклада")
    status.add_argument("--submission-id", required=True)
    status.add_argument("--status", required=True, choices=["submitted", "accepted", "rejected"])

    add_session = subparsers.add_parser("add-session", help="Добавить сессию в расписание")
    add_session.add_argument("--session-id", required=True)
    add_session.add_argument("--track", required=True)
    add_session.add_argument("--room", required=True)
    add_session.add_argument("--start", required=True, type=parse_datetime)
    add_session.add_argument("--end", required=True, type=parse_datetime)

    assign = subparsers.add_parser("assign-paper", help="Назначить доклад в сессию")
    assign.add_argument("--session-id", required=True)
    assign.add_argument("--submission-id", required=True)

    subparsers.add_parser("show-schedule", help="Показать расписание")

    return parser


def _execute_command(args: argparse.Namespace, app: ConferenceService) -> int:
    command = args.command
    try:
        if command == "create-conference":
            conference = app.create_conference(args.name, args.start_date, args.end_date)
            print(
                f"Конференция '{conference.name}' создана ({conference.start_date} -> {conference.end_date})."
            )
        elif command == "show-conference":
            conference = app.get_conference()
            print(f"Конференция: {conference.name}")
            print(f"Даты: {conference.start_date} -> {conference.end_date}")
            print(f"Участники: {len(app.list_participants())}")
            print(f"Доклады: {len(app.list_submissions())}")
            print(f"Сессии: {len(app.list_schedule())}")
        elif command == "register-participant":
            participant = app.register_participant(
                participant_id=args.participant_id,
                full_name=args.full_name,
                email=args.email,
                role=args.role,
            )
            print(f"Участник '{participant.participant_id}' зарегистрирован.")
        elif command == "submit-paper":
            submission = app.submit_paper(
                submission_id=args.submission_id,
                participant_id=args.participant_id,
                title=args.title,
                abstract=args.abstract,
            )
            print(
                f"Доклад '{submission.submission_id}' создан со статусом '{submission.status.value}'."
            )
        elif command == "set-status":
            submission = app.set_submission_status(args.submission_id, args.status)
            print(f"Статус доклада '{submission.submission_id}' теперь '{submission.status.value}'.")
        elif command == "add-session":
            session = app.add_session(
                session_id=args.session_id,
                track=args.track,
                room=args.room,
                start_at=args.start,
                end_at=args.end,
            )
            print(f"Сессия '{session.session_id}' создана.")
        elif command == "assign-paper":
            session = app.assign_submission_to_session(args.session_id, args.submission_id)
            print(
                f"Доклад '{args.submission_id}' назначен в сессию '{session.session_id}'."
            )
        elif command == "show-schedule":
            sessions = app.list_schedule()
            if not sessions:
                print("Расписание пустое.")
            for session in sessions:
                papers = ", ".join(session.submission_ids) if session.submission_ids else "-"
                print(
                    f"{session.session_id}: {session.track} | {session.room} | "
                    f"{session.start_at.isoformat()} -> {session.end_at.isoformat()} | доклады: {papers}"
                )
        else:
            print("Ошибка: неизвестная команда.")
            return 1
    except DomainError as error:
        print(f"Ошибка: {error}")
        return 1
    return 0


def _prompt_text(label: str) -> str:
    return input(f"{label}: ").strip()


def _show_menu() -> None:
    print("\nМеню CLI конференции:")
    print("1. Создать конференцию")
    print("2. Показать конференцию")
    print("3. Зарегистрировать участника")
    print("4. Подать доклад")
    print("5. Изменить статус доклада")
    print("6. Добавить сессию")
    print("7. Назначить доклад в сессию")
    print("8. Показать расписание")
    print("0. Выход")


def run_interactive_menu(service: ConferenceService | None = None) -> int:
    app = service if service is not None else ConferenceService()
    while True:
        try:
            _show_menu()
            choice = _prompt_text("Выберите действие")
        except (EOFError, KeyboardInterrupt):
            print("\nВыход.")
            return 0
        if choice == "0":
            print("Выход.")
            return 0
        try:
            if choice == "1":
                args = argparse.Namespace(
                    command="create-conference",
                    name=_prompt_text("Название конференции"),
                    start_date=parse_date(_prompt_text("Дата начала (YYYY-MM-DD)")),
                    end_date=parse_date(_prompt_text("Дата окончания (YYYY-MM-DD)")),
                )
            elif choice == "2":
                args = argparse.Namespace(command="show-conference")
            elif choice == "3":
                args = argparse.Namespace(
                    command="register-participant",
                    participant_id=_prompt_text("Идентификатор участника"),
                    full_name=_prompt_text("ФИО"),
                    email=_prompt_text("Email"),
                    role=_prompt_text("Роль (speaker/attendee)") or "attendee",
                )
            elif choice == "4":
                args = argparse.Namespace(
                    command="submit-paper",
                    submission_id=_prompt_text("Идентификатор доклада"),
                    participant_id=_prompt_text("Идентификатор участника"),
                    title=_prompt_text("Название доклада"),
                    abstract=_prompt_text("Аннотация"),
                )
            elif choice == "5":
                args = argparse.Namespace(
                    command="set-status",
                    submission_id=_prompt_text("Идентификатор доклада"),
                    status=_prompt_text("Статус (submitted/accepted/rejected)").lower(),
                )
            elif choice == "6":
                args = argparse.Namespace(
                    command="add-session",
                    session_id=_prompt_text("Идентификатор сессии"),
                    track=_prompt_text("Трек"),
                    room=_prompt_text("Аудитория"),
                    start=parse_datetime(_prompt_text("Начало (YYYY-MM-DDTHH:MM)")),
                    end=parse_datetime(_prompt_text("Окончание (YYYY-MM-DDTHH:MM)")),
                )
            elif choice == "7":
                args = argparse.Namespace(
                    command="assign-paper",
                    session_id=_prompt_text("Идентификатор сессии"),
                    submission_id=_prompt_text("Идентификатор доклада"),
                )
            elif choice == "8":
                args = argparse.Namespace(command="show-schedule")
            else:
                print("Неизвестный пункт меню. Повторите ввод.")
                continue
        except argparse.ArgumentTypeError as error:
            print(f"Ошибка: {error}")
            continue
        _execute_command(args, app)


def run_cli(argv: Sequence[str] | None = None, service: ConferenceService | None = None) -> int:
    parser = build_parser()
    try:
        args = parser.parse_args(argv)
    except SystemExit as error:
        if isinstance(error.code, int):
            return error.code
        print(error.code)
        return 2
    app = service if service is not None else ConferenceService()
    if args.command is None:
        return run_interactive_menu(app)
    return _execute_command(args, app)
