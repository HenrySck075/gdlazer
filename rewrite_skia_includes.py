#!/usr/bin/env python3
"""
Rewrite Skia include paths to add 'skia/' prefix.
Finds all #include directives with paths starting with 'include/' 
and rewrites them to start with 'skia/include/' instead.
"""

import os
import re
import sys
from pathlib import Path


def rewrite_includes_in_file(filepath):
    """
    Rewrite include paths in a single file.
    Returns True if file was modified, False otherwise.
    """
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
    except Exception as e:
        print(f"Error reading {filepath}: {e}", file=sys.stderr)
        return False
    
    original_content = content
    
    # Replace #include "include/ with #include "skia/include/
    content = re.sub(
        r'#include\s+"include/',
        '#include "skia/include/',
        content
    )
    
    # Also handle #include <include/ (less common but possible)
    content = re.sub(
        r'#include\s+<include/',
        '#include <skia/include/',
        content
    )
    
    # Handle #include "modules/ with #include "skia/modules/
    content = re.sub(
        r'#include\s+"modules/',
        '#include "skia/modules/',
        content
    )
    
    # Also handle #include <modules/
    content = re.sub(
        r'#include\s+<modules/',
        '#include <skia/modules/',
        content
    )
    
    if content != original_content:
        try:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)
            return True
        except Exception as e:
            print(f"Error writing {filepath}: {e}", file=sys.stderr)
            return False
    
    return False


def main():
    if len(sys.argv) < 2:
        print("Usage: rewrite_skia_includes.py <skia_include_dir>")
        sys.exit(1)
    
    skia_include_dir = Path(sys.argv[1])
    
    if not skia_include_dir.exists():
        print(f"Error: Directory {skia_include_dir} does not exist", file=sys.stderr)
        sys.exit(1)
    
    if not skia_include_dir.is_dir():
        print(f"Error: {skia_include_dir} is not a directory", file=sys.stderr)
        sys.exit(1)
    
    # Find all header files (.h, .hpp)
    header_extensions = {'.h', '.hpp', '.hxx', '.h++'}
    modified_count = 0
    file_count = 0
    
    for filepath in skia_include_dir.rglob('*'):
        if filepath.is_file() and filepath.suffix in header_extensions:
            file_count += 1
            if rewrite_includes_in_file(filepath):
                print(f"Modified: {filepath}")
                modified_count += 1
    
    print(f"\nProcessed {file_count} header files, modified {modified_count} files")


if __name__ == '__main__':
    main()
