import pytest

from student_registry.models import DomainError, create_student_record


def test_create_student_record_rejects_invalid_numbers() -> None:
    with pytest.raises(DomainError):
        create_student_record(
            full_name="Ivan Petrov",
            course=0,
            group_name="A-11",
            total_works=10,
            completed_works=7,
            programming_language="Python",
        )
    with pytest.raises(DomainError):
        create_student_record(
            full_name="Ivan Petrov",
            course=2,
            group_name="A-11",
            total_works=4,
            completed_works=6,
            programming_language="Python",
        )


def test_create_student_record_rejects_empty_strings() -> None:
    with pytest.raises(DomainError):
        create_student_record(
            full_name=" ",
            course=2,
            group_name="A-11",
            total_works=10,
            completed_works=5,
            programming_language="Python",
        )
    with pytest.raises(DomainError):
        create_student_record(
            full_name="Ivan Petrov",
            course=2,
            group_name=" ",
            total_works=10,
            completed_works=5,
            programming_language="Python",
        )
