## 10th November 2026

### 10:00 - 11:36

Set up the CLion project on WSL. Made python env and Python file. Next up is a script for extracting [MineLib](https://minelib.org/v1/).

### 11:42 

Found a [parser](https://github.com/eduardosalaz/minelib-parser/tree/main) online, will try to use it for my own project. 

### 11:50

Before writing my own parser, I need to figure out how I want to represent my data. For that I will consult the [paper](https://www.sciencedirect.com/science/article/abs/pii/S1571065310000533) describing the problem, as well as MineLib documentation.

### 15:20

Had to go for a while, but I came up with a way to store data locally in txt files. My inputs will be:

**Line 1:** [num blocks] [num periods] [profit discount constant]

**Line 2:** [period 1 max resources] ... [period last max resources]

**Two lines for each block:**
- **Line 1:** [block id] [resources required for extraction] [base profit] [number of precedences]
- **Line 2:** [precedence 1] [presedence 2] ... [last precedence]

Now I can download the MineLib and start parsing it