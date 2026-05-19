from pathlib import Path
import sys


def pytest_sessionstart(session) -> None:
    del session
    project_root = Path.cwd()
    source_path = project_root / "src"
    source_path_as_text = str(source_path)
    if source_path_as_text not in sys.path:
        sys.path.insert(0, source_path_as_text)
