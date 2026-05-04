from datetime import date, datetime

import pytest

from conference.models import Conference, DomainError, Participant, Session, Submission


def test_conference_rejects_invalid_dates() -> None:
    with pytest.raises(DomainError):
        Conference(
            name="Applied AI Summit",
            start_date=date(2026, 6, 12),
            end_date=date(2026, 6, 11),
        )


def test_participant_rejects_invalid_email() -> None:
    with pytest.raises(DomainError):
        Participant(
            participant_id="p-101",
            full_name="Elena Petrova",
            email="elenapetrova.mail.com",
        )


def test_session_rejects_invalid_duration() -> None:
    with pytest.raises(DomainError):
        Session(
            session_id="s-ml-01",
            track="Machine Learning",
            room="A-204",
            start_at=datetime(2026, 6, 12, 10, 0),
            end_at=datetime(2026, 6, 12, 10, 0),
        )


def test_submission_rejects_empty_abstract() -> None:
    with pytest.raises(DomainError):
        Submission(
            submission_id="paper-001",
            participant_id="p-101",
            title="Graph Methods for Recommendations",
            abstract="",
        )
