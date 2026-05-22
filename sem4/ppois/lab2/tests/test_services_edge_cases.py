import pytest

from student_registry.models import DomainError, FilterMode, SearchCriteria, create_student_record
from student_registry.services import StudentRegistryService, paginate, record_matches


def _sample_record():
    return create_student_record(
        full_name="Mariya Kuznetsova",
        course=4,
        group_name="P-41",
        total_works=15,
        completed_works=9,
        programming_language="Python",
    )


def test_paginate_validates_per_page_and_normalizes_page() -> None:
    records = [_sample_record()]
    with pytest.raises(DomainError):
        paginate(records, page=1, per_page=0)
    page = paginate(records, page=99, per_page=1)
    assert page.page == 1
    assert page.total_pages == 1


def test_record_matches_validation_errors() -> None:
    record = _sample_record()
    with pytest.raises(DomainError):
        record_matches(record, SearchCriteria(mode=FilterMode.BY_NAME_OR_GROUP, text_query=" "))
    with pytest.raises(DomainError):
        record_matches(record, SearchCriteria(mode=FilterMode.BY_COURSE_OR_LANGUAGE))
    with pytest.raises(DomainError):
        record_matches(
            record,
            SearchCriteria(mode=FilterMode.BY_COURSE_OR_LANGUAGE, use_course=True),
        )
    with pytest.raises(DomainError):
        record_matches(
            record,
            SearchCriteria(mode=FilterMode.BY_COURSE_OR_LANGUAGE, use_course=False, language_value=" "),
        )
    with pytest.raises(DomainError):
        record_matches(record, SearchCriteria(mode=FilterMode.BY_COMPLETED_OR_TOTAL))
    with pytest.raises(DomainError):
        record_matches(
            record,
            SearchCriteria(mode=FilterMode.BY_COMPLETED_OR_TOTAL, use_completed=True),
        )
    with pytest.raises(DomainError):
        record_matches(
            record,
            SearchCriteria(mode=FilterMode.BY_COMPLETED_OR_TOTAL, use_completed=False),
        )
    with pytest.raises(DomainError):
        record_matches(record, SearchCriteria(mode=FilterMode.BY_PENDING))


def test_record_matches_unsupported_mode_branch() -> None:
    record = _sample_record()
    criteria = SearchCriteria(mode=FilterMode.BY_PENDING, pending_value=0)
    criteria.mode = "not-supported"  # type: ignore[assignment]
    with pytest.raises(DomainError):
        record_matches(record, criteria)


def test_service_lists_are_unique_and_sorted() -> None:
    service = StudentRegistryService()
    first = _sample_record()
    second = create_student_record(
        full_name="Ivan Petrov",
        course=2,
        group_name="P-21",
        total_works=12,
        completed_works=9,
        programming_language="Go",
    )
    service.replace_records([first, second])
    assert service.get_languages() == ["Go", "Python"]
    assert service.get_completed_values() == [9]
    assert service.get_total_values() == [12, 15]
