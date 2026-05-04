from __future__ import annotations

from datetime import date, datetime

from conference.models import (
    Conference,
    DomainError,
    Participant,
    Session,
    Submission,
    SubmissionStatus,
)


class ConferenceService:
    def __init__(self) -> None:
        self._conference: Conference | None = None
        self._participants: dict[str, Participant] = {}
        self._submissions: dict[str, Submission] = {}
        self._sessions: dict[str, Session] = {}

    def create_conference(self, name: str, start_date: date, end_date: date) -> Conference:
        if self._conference is not None:
            raise DomainError("Конференция уже создана.")
        self._conference = Conference(name=name, start_date=start_date, end_date=end_date)
        return self._conference

    def get_conference(self) -> Conference:
        if self._conference is None:
            raise DomainError("Конференция еще не создана.")
        return self._conference

    def register_participant(
        self,
        participant_id: str,
        full_name: str,
        email: str,
        role: str = "attendee",
    ) -> Participant:
        self.get_conference()
        if participant_id in self._participants:
            raise DomainError(f"Участник '{participant_id}' уже существует.")
        participant = Participant(
            participant_id=participant_id,
            full_name=full_name,
            email=email,
            role=role,
        )
        self._participants[participant_id] = participant
        return participant

    def submit_paper(
        self,
        submission_id: str,
        participant_id: str,
        title: str,
        abstract: str,
    ) -> Submission:
        self.get_conference()
        if participant_id not in self._participants:
            raise DomainError(f"Участник '{participant_id}' не существует.")
        if submission_id in self._submissions:
            raise DomainError(f"Доклад '{submission_id}' уже существует.")
        submission = Submission(
            submission_id=submission_id,
            participant_id=participant_id,
            title=title,
            abstract=abstract,
        )
        self._submissions[submission_id] = submission
        return submission

    def set_submission_status(
        self,
        submission_id: str,
        status: str | SubmissionStatus,
    ) -> Submission:
        self.get_conference()
        submission = self._submissions.get(submission_id)
        if submission is None:
            raise DomainError(f"Доклад '{submission_id}' не существует.")
        normalized_status = self._parse_status(status)
        submission.status = normalized_status
        return submission

    def add_session(
        self,
        session_id: str,
        track: str,
        room: str,
        start_at: datetime,
        end_at: datetime,
    ) -> Session:
        self.get_conference()
        if session_id in self._sessions:
            raise DomainError(f"Сессия '{session_id}' уже существует.")
        new_session = Session(
            session_id=session_id,
            track=track,
            room=room,
            start_at=start_at,
            end_at=end_at,
        )
        for existing in self._sessions.values():
            if not self._has_overlap(new_session.start_at, new_session.end_at, existing.start_at, existing.end_at):
                continue
            if existing.room == new_session.room or existing.track == new_session.track:
                raise DomainError(
                    f"Сессия '{session_id}' пересекается с сессией '{existing.session_id}'."
                )
        self._sessions[session_id] = new_session
        return new_session

    def assign_submission_to_session(self, session_id: str, submission_id: str) -> Session:
        self.get_conference()
        session = self._sessions.get(session_id)
        if session is None:
            raise DomainError(f"Сессия '{session_id}' не существует.")
        submission = self._submissions.get(submission_id)
        if submission is None:
            raise DomainError(f"Доклад '{submission_id}' не существует.")
        if submission.status != SubmissionStatus.ACCEPTED:
            raise DomainError("В расписание можно назначать только принятые доклады.")
        if submission_id in session.submission_ids:
            raise DomainError(
                f"Доклад '{submission_id}' уже назначен в сессию '{session_id}'."
            )
        session.submission_ids.append(submission_id)
        return session

    def list_schedule(self) -> list[Session]:
        self.get_conference()
        return sorted(self._sessions.values(), key=lambda item: item.start_at)

    def list_participants(self) -> list[Participant]:
        self.get_conference()
        return sorted(self._participants.values(), key=lambda item: item.participant_id)

    def list_submissions(self) -> list[Submission]:
        self.get_conference()
        return sorted(self._submissions.values(), key=lambda item: item.submission_id)

    @staticmethod
    def _parse_status(status: str | SubmissionStatus) -> SubmissionStatus:
        if isinstance(status, SubmissionStatus):
            return status
        try:
            return SubmissionStatus(status.lower())
        except ValueError as error:
            raise DomainError(f"Неподдерживаемый статус доклада '{status}'.") from error

    @staticmethod
    def _has_overlap(
        first_start: datetime,
        first_end: datetime,
        second_start: datetime,
        second_end: datetime,
    ) -> bool:
        return max(first_start, second_start) < min(first_end, second_end)
