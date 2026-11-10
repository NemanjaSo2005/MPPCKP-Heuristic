# Script that downloads all minelib mines into ./data/MineLib

import requests
from pathlib import Path

mine_names = ["newman1",
              "zuck_small",
              "kd",
              "zuck_medium",
              "p4hd",
              "marvin",
              "w23",
              "zuck_large",
              "sm2",
              "mclaughlin_limit",
              "mclaughlin"]
file_extensions = ["blocks",
                   "prec",
                   "cpit"]

def download_file(url, path):
    response = requests.get(url)
    response.raise_for_status()

    path.write_bytes(response.content)

minelib_folder = Path("./data/MineLib")
minelib_folder.mkdir(parents = True, exist_ok = True)

for mine in mine_names:
    for extension in file_extensions:
        try:
            file_name = mine + "." + extension
            mine_path = minelib_folder / file_name
            if mine_path.is_file():
                print(f"File {mine_path} already exists")
            else:
                print(f"Donwloading {mine_path}")
                download_file("https://minelib.org/v1/data/" + file_name, mine_path)
        except requests.RequestException as e:
            print(f"Failed to download {mine}.{extension}")