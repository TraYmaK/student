from student_registry.controller import AppController
from student_registry.services import StudentRegistryService


def run_app() -> None:
    service = StudentRegistryService()
    controller = AppController(service)
    controller.run()
