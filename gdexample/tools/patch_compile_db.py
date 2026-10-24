#!/usr/bin/env python3
"""Add macOS case aliases used by editors to the gdexample compile database."""

import json
import sys
from pathlib import Path


def main() -> int:
    if len(sys.argv) != 2:
        print(f"usage: {Path(sys.argv[0]).name} COMPILE_DATABASE", file=sys.stderr)
        return 2

    database_path = Path(sys.argv[1])
    entries = json.loads(database_path.read_text(encoding="utf-8"))
    aliases = {
        "src/game/Game.cpp": "src/game/game.cpp",
    }

    existing = {(entry.get("directory"), entry.get("file")) for entry in entries}
    for entry in list(entries):
        alias = aliases.get(entry.get("file"))
        key = (entry.get("directory"), alias)
        if alias and key not in existing:
            alias_entry = dict(entry)
            alias_entry["file"] = alias
            entries.append(alias_entry)
            existing.add(key)

    database_path.write_text(
        json.dumps(entries, indent=2, ensure_ascii=True) + "\n",
        encoding="utf-8",
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
