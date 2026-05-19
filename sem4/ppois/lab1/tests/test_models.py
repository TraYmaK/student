from datetime import date, datetime

import pytest

from conference.models import (
    SubmissionStatus,
    DomainError,
    create_conference,
    create_participant,
    create_session,
    create_submission,
)


def test_conference_rejects_invalid_dates() -> None:
    with pytest.raises(DomainError):
        create_conference(
            name="Applied AI Summit",
            start_date=date(2026, 6, 12),
            end_date=date(2026, 6, 11),
        )


def test_participant_rejects_invalid_email() -> None:
    with pytest.raises(DomainError):
        create_participant(
            participant_id="p-101",
            full_name="Elena Petrova",
            email="elenapetrova.mail.com",
        )


def test_session_rejects_invalid_duration() -> None:
    with pytest.raises(DomainError):
        create_session(
            session_id="s-ml-01",
            track="Machine Learning",
            room="A-204",
            start_at=datetime(2026, 6, 12, 10, 0),
            end_at=datetime(2026, 6, 12, 10, 0),
        )


def test_submission_rejects_empty_abstract() -> None:
    with pytest.raises(DomainError):
        create_submission(
            submission_id="paper-001",
            participant_id="p-101",
            title="Graph Methods for Recommendations",
            abstract="",
        )


def test_conference_rejects_empty_name() -> None:
    with pytest.raises(DomainError):
        create_conference(
            name="   ",
            start_date=date(2026, 6, 12),
            end_date=date(2026, 6, 13),
        )


def test_participant_rejects_empty_full_name_and_role() -> None:
    with pytest.raises(DomainError):
        create_participant(
            participant_id="p-102",
            full_name="",
            email="elena@example.com",
        )
    with pytest.raises(DomainError):
        create_participant(
            participant_id="p-103",
            full_name="Elena Petrova",
            email="elena@example.com",
            role=" ",
        )


def test_participant_rejects_empty_identifier() -> None:
    with pytest.raises(DomainError):
        create_participant(
            participant_id=" ",
            full_name="Elena Petrova",
            email="elena@example.com",
        )


def test_submission_rejects_empty_identifiers_and_title() -> None:
    with pytest.raises(DomainError):
        create_submission(
            submission_id="",
            participant_id="p-101",
            title="Title",
            abstract="Abstract",
        )
    with pytest.raises(DomainError):
        create_submission(
            submission_id="paper-001",
            participant_id=" ",
            title="Title",
            abstract="Abstract",
        )
    with pytest.raises(DomainError):
        create_submission(
            submission_id="paper-001",
            participant_id="p-101",
            title=" ",
            abstract="Abstract",
        )


def test_create_submission_accepts_custom_status() -> None:
    submission = create_submission(
        submission_id="paper-accepted",
        participant_id="p-101",
        title="Graph Methods",
        abstract="Valid abstract text",
        status=SubmissionStatus.ACCEPTED,
    )
    assert submission.status is SubmissionStatus.ACCEPTED


def test_session_rejects_empty_fields_and_duplicate_submission_ids() -> None:
    with pytest.raises(DomainError):
        create_session(
            session_id=" ",
            track="ML",
            room="A-101",
            start_at=datetime(2026, 6, 12, 10, 0),
            end_at=datetime(2026, 6, 12, 11, 0),
        )
    with pytest.raises(DomainError):
        create_session(
            session_id="s-01",
            track=" ",
            room="A-101",
            start_at=datetime(2026, 6, 12, 10, 0),
            end_at=datetime(2026, 6, 12, 11, 0),
        )
    with pytest.raises(DomainError):
        create_session(
            session_id="s-01",
            track="ML",
            room=" ",
            start_at=datetime(2026, 6, 12, 10, 0),
            end_at=datetime(2026, 6, 12, 11, 0),
        )
    with pytest.raises(DomainError):
        create_session(
            session_id="s-02",
            track="ML",
            room="A-101",
            start_at=datetime(2026, 6, 12, 10, 0),
            end_at=datetime(2026, 6, 12, 11, 0),
            submission_ids=["paper-1", "paper-1"],
        )
