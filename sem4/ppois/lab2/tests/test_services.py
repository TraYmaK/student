import pytest

from student_registry.models import FilterMode, SearchCriteria, create_student_record
from student_registry.services import StudentRegistryService, paginate


@pytest.fixture()
def service() -> StudentRegistryService:
    app = StudentRegistryService()
    app.add_record(
        create_student_record(
            full_name="Ivan Petrov",
            course=2,
            group_name="P-21",
            total_works=12,
            completed_works=8,
            programming_language="Python",
        )
    )
    app.add_record(
        create_student_record(
            full_name="Anna Smirnova",
            course=3,
            group_name="P-31",
            total_works=10,
            completed_works=10,
            programming_language="Java",
        )
    )
    app.add_record(
        create_student_record(
            full_name="Dmitry Orlov",
            course=2,
            group_name="P-22",
            total_works=12,
            completed_works=7,
            programming_language="Python",
        )
    )
    return app


def test_search_by_name_or_group(service: StudentRegistryService) -> None:
    result = service.search(SearchCriteria(mode=FilterMode.BY_NAME_OR_GROUP, text_query="P-2"))
    assert len(result) == 2


def test_search_by_course_or_language(service: StudentRegistryService) -> None:
    result_by_course = service.search(
        SearchCriteria(
            mode=FilterMode.BY_COURSE_OR_LANGUAGE,
            use_course=True,
            course_value=3,
        )
    )
    assert [item.full_name for item in result_by_course] == ["Anna Smirnova"]
    result_by_language = service.search(
        SearchCriteria(
            mode=FilterMode.BY_COURSE_OR_LANGUAGE,
            use_course=False,
            language_value="python",
        )
    )
    assert len(result_by_language) == 2


def test_search_by_completed_total_and_pending(service: StudentRegistryService) -> None:
    by_completed = service.search(
        SearchCriteria(
            mode=FilterMode.BY_COMPLETED_OR_TOTAL,
            use_completed=True,
            completed_value=10,
        )
    )
    assert [item.full_name for item in by_completed] == ["Anna Smirnova"]
    by_total = service.search(
        SearchCriteria(
            mode=FilterMode.BY_COMPLETED_OR_TOTAL,
            use_completed=False,
            total_value=12,
        )
    )
    assert len(by_total) == 2
    by_pending = service.search(SearchCriteria(mode=FilterMode.BY_PENDING, pending_value=4))
    assert [item.full_name for item in by_pending] == ["Ivan Petrov"]


def test_delete_returns_removed_count(service: StudentRegistryService) -> None:
    removed = service.delete(
        SearchCriteria(
            mode=FilterMode.BY_COURSE_OR_LANGUAGE,
            use_course=False,
            language_value="Python",
        )
    )
    assert removed == 2
    assert len(service.list_records()) == 1


def test_paginate_returns_expected_chunk(service: StudentRegistryService) -> None:
    page = paginate(service.list_records(), page=2, per_page=2)
    assert len(page.items) == 1
    assert page.total_pages == 2
    assert page.page == 2
