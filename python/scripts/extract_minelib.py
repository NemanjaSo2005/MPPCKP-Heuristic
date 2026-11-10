# Takes MineLib found in ./data/MineLib and converts the instances into format used by the heuristic
# As MineLib mines have restrictions on multiple resources, but MPPCKP has only one item weight,
# we make one file for each resource

import math

from pathlib import Path

from minelib import  parse_cpit, parse_precedences

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

minelib_folder = Path("./data/MineLib")

for mine in mine_names:
    print(f"Extracting files for mine {mine}")
    precedences = parse_precedences(minelib_folder / (mine + ".prec"))
    cpit_data = parse_cpit(minelib_folder / (mine + ".cpit"), prec_source=precedences)

    num_blocks = cpit_data.num_blocks
    num_periods = cpit_data.num_periods
    discount_rate = cpit_data.discount_rate

    for r in range(cpit_data.num_resources):
        max_cost_per_period = []
        for t in range(num_periods):
            _, max_cost = cpit_data.resource_limits.get_bounds(r, t)
            max_cost_per_period.append(round(max_cost))

        blocks = []

        for b in range(num_blocks):
            block_id = b
            resource_requirement = cpit_data.get_resource_coefficient(b, r)
            base_profit = cpit_data.objective[block_id]
            block_precedences = precedences.get_predecessors(block_id)
            precedence_count = len(block_precedences)

            block = {
                "id" : block_id,
                "weight" : round(resource_requirement),
                "profit" : round(base_profit),
                "precedence_count" : precedence_count,
                "precedences" : block_precedences
            }

            blocks.append(block)

        destinationFile = Path("./data/ExtractedMineLib/" + mine + f"_{r}.in")
        destinationFile.parent.mkdir(parents=True, exist_ok=True)

        with open(destinationFile, "w") as f:
            f.write(f"{num_blocks} {num_periods} {discount_rate}\n")
            f.write(' '.join(map(str, max_cost_per_period)))
            f.write("\n")

            for block in blocks:
                f.write(f"{block['id']} {block['weight']} {block['profit']} {block['precedence_count']}\n")
                f.write(' '.join(map(str, block['precedences'])))
                f.write("\n")