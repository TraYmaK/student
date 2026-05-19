from dataclasses import dataclass, field
from datetime import date, datetime
from enum import Enum


class DomainError(ValueError):
    """Базовая ошибка для валидации и некорректных состояний модели."""


class SubmissionStatus(str, Enum):
    SUBMITTED = "submitted"
    ACCEPTED = "accepted"
    REJECTED = "rejected"


@dataclass(slots=True)
class Conference:
    name: str
    start_date: date
    end_date: date


@dataclass(slots=True)
class Participant:
    participant_id: str
    full_name: str
    email: str
    role: str = "attendee"


@dataclass(slots=True)
class Submission:
    submission_id: str
    participant_id: str
    title: str
    abstract: str
    status: SubmissionStatus = SubmissionStatus.SUBMITTED


@dataclass(slots=True)
class Session:
    session_id: str
    track: str
    room: str
    start_at: datetime
    end_at: datetime
    submission_ids: list[str] = field(default_factory=list)


def create_conference(name: str, start_date: date, end_date: date) -> Conference:
    if not name.strip():
        raise DomainError("Название конференции не должно быть пустым.")
    if end_date < start_date:
        raise DomainError("Дата окончания конференции должна быть позже даты начала.")
    return Conference(name=name, start_date=start_date, end_date=end_date)


def create_participant(
    participant_id: str,
    full_name: str,
    email: str,
    role: str = "attendee",
) -> Participant:
    if not participant_id.strip():
        raise DomainError("Идентификатор участника не должен быть пустым.")
    if not full_name.strip():
        raise DomainError("ФИО участника не должно быть пустым.")
    if "@" not in email or email.startswith("@") or email.endswith("@"):
        raise DomainError("Email участника некорректен.")
    if not role.strip():
        raise DomainError("Роль участника не должна быть пустой.")
    return Participant(
        participant_id=participant_id,
        full_name=full_name,
        email=email,
        role=role,
    )


def create_submission(
    submission_id: str,
    participant_id: str,
    title: str,
    abstract: str,
    status: SubmissionStatus = SubmissionStatus.SUBMITTED,
) -> Submission:
    if not submission_id.strip():
        raise DomainError("Идентификатор доклада не должен быть пустым.")
    if not participant_id.strip():
        raise DomainError("Идентификатор участника не должен быть пустым.")
    if not title.strip():
        raise DomainError("Название доклада не должно быть пустым.")
    if not abstract.strip():
        raise DomainError("Аннотация доклада не должна быть пустой.")
    return Submission(
        submission_id=submission_id,
        participant_id=participant_id,
        title=title,
        abstract=abstract,
        status=status,
    )


def create_session(
    session_id: str,
    track: str,
    room: str,
    start_at: datetime,
    end_at: datetime,
    submission_ids: list[str] | None = None,
) -> Session:
    if not session_id.strip():
        raise DomainError("Идентификатор сессии не должен быть пустым.")
    if not track.strip():
        raise DomainError("Название трека не должно быть пустым.")
    if not room.strip():
        raise DomainError("Аудитория сессии не должна быть пустой.")
    if end_at <= start_at:
        raise DomainError("Время окончания сессии должно быть позже времени начала.")
    normalized_submission_ids = submission_ids if submission_ids is not None else []
    if len(normalized_submission_ids) != len(set(normalized_submission_ids)):
        raise DomainError("Идентификаторы докладов в сессии должны быть уникальны.")
    return Session(
        session_id=session_id,
        track=track,
        room=room,
        start_at=start_at,
        end_at=end_at,
        submission_ids=normalized_submission_ids,
    )
