import subprocess as sp

TARGET = './a.out'
ROUNDS = int(input("Number of rounds: "))

red_win = 0
blue_win = 0

for i in range(ROUNDS):
    out = sp.run(TARGET, capture_output=True).stdout
    if b'Red Player won' in out:
        red_win += 1
    elif b'Blue Player won' in out:
        blue_win += 1
    else:
        print(out)
    if (i % (ROUNDS / 10) == 0):
        print(f'{i / ROUNDS * 100}%')

print(f'Red : {red_win} ({red_win / ROUNDS * 100}%)')
print(f'Blue: {blue_win} ({blue_win / ROUNDS * 100}%)')
