import subprocess, json
import os

file_dir = os.path.dirname(os.path.realpath(__file__))

e = json.loads(
    subprocess.check_output([
        'gh', 
        'api',
         '/repos/ppy/osu/commits?per_page=3'
    ],shell=True)
)

cc = open(os.path.join(file_dir, "current_commit")).read()

if cc!=e[0]["sha"]:
    print("New commit chat!")
    print(f"Commit message: {e[0]["commit"]["message"]}")
    print(f"Check out the diffs on https://github.com/ppy/osu/compare/{cc}..{e[0]["sha"]}")