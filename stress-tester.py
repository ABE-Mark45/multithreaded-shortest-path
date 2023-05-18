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


def test_clients(max_num_of_clients):
    runtimes = {'bfs': [], 'cached_bfs': []}

    for num_of_clients in range(1, 1 + max_num_of_clients):
        for i in range(num_of_clients):
            os.system(f'python generate-client-batch.py input/client-{i}-input.txt --batches 5 --ops 256')

        for stratergy in ['bfs', 'cached_bfs']:
            server = subprocess.Popen(['./build/bin/Server', stratergy, 'assets/graph.txt', 'logs/server-logs.txt'])
            time.sleep(1)

            clients = []
            for i in range(num_of_clients):
                clients.append(subprocess.Popen(['./build/bin/Client', f'client-{i}', f'input/client-{i}-input.txt', f'logs/client-{i}-logs.txt']))
            
            for client in clients:
                client.wait()
            server.kill()

            stratergy_runtimes = [analyze(f'logs/client-{i}-logs.txt') for i in range(num_of_clients)]
            runtimes[stratergy].append(np.max(stratergy_runtimes))
        print(runtimes)

test_clients(15)