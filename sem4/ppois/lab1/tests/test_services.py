from datetime import date, datetime

import pytest

from conference.models import DomainError, SubmissionStatus
from conference.services import ConferenceService, has_overlap, parse_status


@pytest.fixture()
def service() -> ConferenceService:
    app = ConferenceService()
    app.create_conference(
        name="Data Systems Conference",
        start_date=date(2026, 5, 20),
        end_date=date(2026, 5, 22),
    )
    return app


def test_register_participant_fails_for_duplicate_id(service: ConferenceService) -> None:
    service.register_participant(
        participant_id="p-200",
        full_name="Anton Smirnov",
        email="anton.smirnov@example.com",
    )

    with pytest.raises(DomainError):
        service.register_participant(
            participant_id="p-200",
            full_name="Irina Voronova",
            email="irina.voronova@example.com",
        )


def test_submit_paper_fails_for_unknown_participant(service: ConferenceService) -> None:
    with pytest.raises(DomainError):
        service.submit_paper(
            submission_id="paper-123",
            participant_id="missing-participant",
            title="Secure Federated Learning",
            abstract="Realistic benchmark on healthcare data.",
        )


def test_schedule_fails_for_overlapping_session_in_same_room(service: ConferenceService) -> None:
    service.add_session(
        session_id="s-main-01",
        track="AI Systems",
        room="B-101",
        start_at=datetime(2026, 5, 20, 10, 0),
        end_at=datetime(2026, 5, 20, 11, 0),
    )

    with pytest.raises(DomainError):
        service.add_session(
            session_id="s-main-02",
            track="Security",
            room="B-101",
            start_at=datetime(2026, 5, 20, 10, 30),
            end_at=datetime(2026, 5, 20, 11, 30),
        )


def test_assign_submission_requires_accepted_status(service: ConferenceService) -> None:
    service.register_participant(
        participant_id="p-301",
        full_name="Mariya Kuznetsova",
        email="mariya.kuznetsova@example.com",
    )
    service.submit_paper(
        submission_id="paper-777",
        participant_id="p-301",
        title="Cloud-Native Stream Processing",
        abstract="Latency analysis for production pipelines.",
    )
    service.add_session(
        session_id="s-cloud-01",
        track="Cloud",
        room="A-301",
        start_at=datetime(2026, 5, 20, 13, 0),
        end_at=datetime(2026, 5, 20, 14, 0),
    )

    with pytest.raises(DomainError):
        service.assign_submission_to_session("s-cloud-01", "paper-777")

    service.set_submission_status("paper-777", SubmissionStatus.ACCEPTED)
    session = service.assign_submission_to_session("s-cloud-01", "paper-777")
    assert session.submission_ids == ["paper-777"]


def test_happy_path_creates_sorted_schedule(service: ConferenceService) -> None:
    service.add_session(
        session_id="s-2",
        track="Databases",
        room="C-201",
        start_at=datetime(2026, 5, 20, 14, 0),
        end_at=datetime(2026, 5, 20, 15, 0),
    )
    service.add_session(
        session_id="s-1",
        track="Machine Learning",
        room="C-101",
        start_at=datetime(2026, 5, 20, 9, 0),
        end_at=datetime(2026, 5, 20, 10, 0),
    )
    schedule = service.list_schedule()
    assert [session.session_id for session in schedule] == ["s-1", "s-2"]


def test_service_requires_conference_for_read_operations() -> None:
    service = ConferenceService()
    with pytest.raises(DomainError):
        service.get_conference()


def test_service_rejects_duplicate_conference_creation() -> None:
    service = ConferenceService()
    service.create_conference(
        name="Systems Week",
        start_date=date(2026, 9, 1),
        end_date=date(2026, 9, 2),
    )
    with pytest.raises(DomainError):
        service.create_conference(
            name="Systems Week 2",
            start_date=date(2026, 10, 1),
            end_date=date(2026, 10, 2),
        )


def test_submit_paper_fails_for_duplicate_id(service: ConferenceService) -> None:
    service.register_participant(
        participant_id="p-401",
        full_name="Kirill Egorov",
        email="kirill.egorov@example.com",
    )
    service.submit_paper(
        submission_id="paper-duplicate",
        participant_id="p-401",
        title="First title",
        abstract="First abstract",
    )
    with pytest.raises(DomainError):
        service.submit_paper(
            submission_id="paper-duplicate",
            participant_id="p-401",
            title="Second title",
            abstract="Second abstract",
        )


def test_set_submission_status_fails_for_unknown_submission(service: ConferenceService) -> None:
    with pytest.raises(DomainError):
        service.set_submission_status("missing-paper", "accepted")


def test_set_submission_status_fails_for_invalid_status(service: ConferenceService) -> None:
    service.register_participant(
        participant_id="p-402",
        full_name="Olga Sidorova",
        email="olga.sidorova@example.com",
    )
    service.submit_paper(
        submission_id="paper-invalid-status",
        participant_id="p-402",
        title="Invalid status demo",
        abstract="Detailed abstract text",
    )
    with pytest.raises(DomainError):
        service.set_submission_status("paper-invalid-status", "in-review")


def test_add_session_rejects_duplicate_identifier(service: ConferenceService) -> None:
    service.add_session(
        session_id="dup-session",
        track="Backend",
        room="A-110",
        start_at=datetime(2026, 5, 20, 8, 0),
        end_at=datetime(2026, 5, 20, 9, 0),
    )
    with pytest.raises(DomainError):
        service.add_session(
            session_id="dup-session",
            track="Backend 2",
            room="A-120",
            start_at=datetime(2026, 5, 20, 10, 0),
            end_at=datetime(2026, 5, 20, 11, 0),
        )


def test_assign_submission_validates_session_and_duplicate_assignment(service: ConferenceService) -> None:
    service.register_participant(
        participant_id="p-501",
        full_name="Pavel Orlov",
        email="pavel.orlov@example.com",
    )
    service.submit_paper(
        submission_id="paper-501",
        participant_id="p-501",
        title="Event processing",
        abstract="A realistic abstract",
    )
    service.set_submission_status("paper-501", "accepted")

    with pytest.raises(DomainError):
        service.assign_submission_to_session("missing-session", "paper-501")

    service.add_session(
        session_id="s-assign-01",
        track="Eventing",
        room="E-201",
        start_at=datetime(2026, 5, 20, 16, 0),
        end_at=datetime(2026, 5, 20, 17, 0),
    )
    service.assign_submission_to_session("s-assign-01", "paper-501")
    with pytest.raises(DomainError):
        service.assign_submission_to_session("s-assign-01", "paper-501")


def test_parse_status_and_overlap_helpers_cover_all_branches() -> None:
    assert parse_status(SubmissionStatus.ACCEPTED) is SubmissionStatus.ACCEPTED
    assert parse_status("accepted") is SubmissionStatus.ACCEPTED
    with pytest.raises(DomainError):
        parse_status("not-real")

    assert has_overlap(
        datetime(2026, 5, 20, 9, 0),
        datetime(2026, 5, 20, 10, 0),
        datetime(2026, 5, 20, 9, 30),
        datetime(2026, 5, 20, 11, 0),
    )
    assert not has_overlap(
        datetime(2026, 5, 20, 9, 0),
        datetime(2026, 5, 20, 10, 0),
        datetime(2026, 5, 20, 10, 0),
        datetime(2026, 5, 20, 11, 0),
    )
