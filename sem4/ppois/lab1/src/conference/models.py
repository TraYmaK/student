from __future__ import annotations

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

    def __post_init__(self) -> None:
        if not self.name.strip():
            raise DomainError("Название конференции не должно быть пустым.")
        if self.end_date < self.start_date:
            raise DomainError("Дата окончания конференции должна быть позже даты начала.")


@dataclass(slots=True)
class Participant:
    participant_id: str
    full_name: str
    email: str
    role: str = "attendee"

    def __post_init__(self) -> None:
        if not self.participant_id.strip():
            raise DomainError("Идентификатор участника не должен быть пустым.")
        if not self.full_name.strip():
            raise DomainError("ФИО участника не должно быть пустым.")
        if "@" not in self.email or self.email.startswith("@") or self.email.endswith("@"):
            raise DomainError("Email участника некорректен.")
        if not self.role.strip():
            raise DomainError("Роль участника не должна быть пустой.")


@dataclass(slots=True)
class Submission:
    submission_id: str
    participant_id: str
    title: str
    abstract: str
    status: SubmissionStatus = SubmissionStatus.SUBMITTED

    def __post_init__(self) -> None:
        if not self.submission_id.strip():
            raise DomainError("Идентификатор доклада не должен быть пустым.")
        if not self.participant_id.strip():
            raise DomainError("Идентификатор участника не должен быть пустым.")
        if not self.title.strip():
            raise DomainError("Название доклада не должно быть пустым.")
        if not self.abstract.strip():
            raise DomainError("Аннотация доклада не должна быть пустой.")


@dataclass(slots=True)
class Session:
    session_id: str
    track: str
    room: str
    start_at: datetime
    end_at: datetime
    submission_ids: list[str] = field(default_factory=list)

    def __post_init__(self) -> None:
        if not self.session_id.strip():
            raise DomainError("Идентификатор сессии не должен быть пустым.")
        if not self.track.strip():
            raise DomainError("Название трека не должно быть пустым.")
        if not self.room.strip():
            raise DomainError("Аудитория сессии не должна быть пустой.")
        if self.end_at <= self.start_at:
            raise DomainError("Время окончания сессии должно быть позже времени начала.")
        if len(self.submission_ids) != len(set(self.submission_ids)):
            raise DomainError("Идентификаторы докладов в сессии должны быть уникальны.")
