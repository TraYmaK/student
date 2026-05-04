from __future__ import annotations

from pathlib import Path

# Make `python -m conference` work from repository root without installation.
_src_package_path = Path(__file__).resolve().parent.parent / "src" / "conference"
if _src_package_path.is_dir():
    __path__.append(str(_src_package_path))
