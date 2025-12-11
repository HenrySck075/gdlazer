#!/usr/bin/env python3
"""
General-purpose cache for Flutter source code.
Allows fetching and caching any files from the Flutter repository.
"""

import os
import sys
import urllib.request
from pathlib import Path

FLUTTER_REPO = "https://raw.githubusercontent.com/flutter/flutter/master/packages/flutter/lib/src"
CACHE_DIR = Path.home() / ".cache" / "flutter_source"

def ensure_cache_dir():
    """Create cache directory if it doesn't exist."""
    CACHE_DIR.mkdir(parents=True, exist_ok=True)

def fetch_file(relative_path: str, force: bool = False) -> Path | None:
    """
    Fetch a file from Flutter repo and cache it locally.
    
    Args:
        relative_path: Path relative to flutter/lib/src (e.g., 'widgets/framework.dart')
        force: Re-fetch even if already cached
    
    Returns:
        Path to cached file, or None if fetch failed
    """
    ensure_cache_dir()
    
    cache_path = CACHE_DIR / relative_path
    
    # Return cached if exists and not forcing
    if cache_path.exists() and not force:
        return cache_path
    
    # Create parent directories
    cache_path.parent.mkdir(parents=True, exist_ok=True)
    
    # Build URL and fetch
    url = f"{FLUTTER_REPO}/{relative_path}"
    try:
        print(f"Fetching {relative_path}...", end=" ", flush=True)
        urllib.request.urlretrieve(url, cache_path)
        size_kb = cache_path.stat().st_size / 1024
        print(f"✓ ({size_kb:.1f} KB)")
        return cache_path
    except Exception as e:
        print(f"✗ Error: {e}")
        if cache_path.exists():
            cache_path.unlink()
        return None

def get_cached(relative_path: str) -> Path | None:
    """Get path to cached file if it exists, without fetching."""
    cache_path = CACHE_DIR / relative_path
    return cache_path if cache_path.exists() else None

def search_cache(pattern: str) -> list[Path]:
    """Search for files in cache by name pattern."""
    ensure_cache_dir()
    results = []
    for path in CACHE_DIR.rglob(f"*{pattern}*"):
        if path.is_file():
            results.append(path)
    return results

def list_cache() -> list[Path]:
    """List all cached files."""
    ensure_cache_dir()
    files = list(CACHE_DIR.rglob("*"))
    return sorted([f for f in files if f.is_file()])

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage:")
        print(f"  {sys.argv[0]} <relative/path> [-f|--force]     Fetch and cache a file")
        print(f"  {sys.argv[0]} --get <relative/path>             Get path to cached file")
        print(f"  {sys.argv[0]} --search <pattern>                Search cached files")
        print(f"  {sys.argv[0]} --list                            List all cached files")
        print(f"\nCache location: {CACHE_DIR}")
        sys.exit(1)
    
    cmd = sys.argv[1]
    
    if cmd == "--get":
        if len(sys.argv) < 3:
            print("Usage: --get <relative/path>")
            sys.exit(1)
        path = get_cached(sys.argv[2])
        if path:
            print(path)
        else:
            print(f"Not cached: {sys.argv[2]}", file=sys.stderr)
            sys.exit(1)
    
    elif cmd == "--search":
        if len(sys.argv) < 3:
            print("Usage: --search <pattern>")
            sys.exit(1)
        results = search_cache(sys.argv[2])
        for path in results:
            print(path.relative_to(CACHE_DIR))
    
    elif cmd == "--list":
        files = list_cache()
        if files:
            for path in files:
                print(path.relative_to(CACHE_DIR))
        else:
            print("Cache is empty")
    
    else:
        # Treat as a fetch command
        force = "-f" in sys.argv or "--force" in sys.argv
        path = fetch_file(cmd, force=force)
        if path:
            print(f"Available at: {path}")
            sys.exit(0)
        else:
            sys.exit(1)
