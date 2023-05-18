import os, subprocess, re
import time
import numpy as np


def analyze(path):
    readings = []
    with open(path, 'r') as f:
        for line in f:
            if line.startswith('Batch was calculated in'):
                readings.append(int(re.findall(r'\d+', line)[0]))
    return np.median(readings)

client_log_path = 'logs/client-1-logs.txt'

# sizes = [(1<<i) for i in range(12)]

ratios = [0.1 * i for i in range(10 + 1)]

runtimes = {
    'bfs': [],
    'cached_bfs': []
}

for ratio in ratios:
    os.system(f'python generate-client-batch.py client-1-input.txt --batches 1 --ops 256 --read-ratio {ratio}')
    for stratergy in ['bfs', 'cached_bfs']:
        readings = []
        for itr in range(5):
            server = subprocess.Popen(['./build/bin/Server', stratergy, 'assets/graph.txt', 'logs/server-logs.txt'])
            time.sleep(1)
            client = subprocess.Popen(['./build/bin/Client', 'client-1', 'client-1-input.txt', client_log_path])
            client.wait()
            server.kill()
            readings.append(analyze(client_log_path))
        runtimes[stratergy].append(np.median(readings))
        print(runtimes)