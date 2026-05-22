import importlib
import sys
import types


def test_run_app_creates_controller_and_runs(monkeypatch) -> None:
    calls = {"run": 0, "service_type": None}
    fake_controller_module = types.ModuleType("student_registry.controller")

    class FakeController:
        def __init__(self, service) -> None:
            calls["service_type"] = type(service).__name__

        def run(self) -> None:
            calls["run"] += 1

    fake_controller_module.AppController = FakeController
    monkeypatch.setitem(sys.modules, "student_registry.controller", fake_controller_module)
    app_module = importlib.import_module("student_registry.app")
    app_module = importlib.reload(app_module)
    app_module.run_app()
    assert calls["run"] == 1
    assert calls["service_type"] == "StudentRegistryService"
