import pytest

from student_registry.models import DomainError, create_student_record


def test_create_student_record_rejects_negative_numbers() -> None:
    with pytest.raises(DomainError):
        create_student_record(
            full_name="Ivan Petrov",
            course=2,
            group_name="P-21",
            total_works=-1,
            completed_works=0,
            programming_language="Python",
        )
    with pytest.raises(DomainError):
        create_student_record(
            full_name="Ivan Petrov",
            course=2,
            group_name="P-21",
            total_works=5,
            completed_works=-1,
            programming_language="Python",
        )


def test_create_student_record_rejects_empty_language_and_out_of_range_course() -> None:
    with pytest.raises(DomainError):
        create_student_record(
            full_name="Ivan Petrov",
            course=7,
            group_name="P-21",
            total_works=5,
            completed_works=1,
            programming_language="Python",
        )
    with pytest.raises(DomainError):
        create_student_record(
            full_name="Ivan Petrov",
            course=2,
            group_name="P-21",
            total_works=5,
            completed_works=1,
            programming_language=" ",
        )


def test_create_student_record_trims_fields_and_computes_pending() -> None:
    record = create_student_record(
        full_name="  Anna Smirnova  ",
        course=3,
        group_name="  P-31  ",
        total_works=11,
        completed_works=7,
        programming_language="  Java  ",
    )
    assert record.full_name == "Anna Smirnova"
    assert record.group_name == "P-31"
    assert record.programming_language == "Java"
    assert record.pending_works == 4
