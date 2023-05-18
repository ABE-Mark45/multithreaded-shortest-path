import matplotlib.pyplot as plt
import numpy as np

# runtimes = {
#     'bfs': np.array([3035.0, 3717.0, 3156.0, 5634.0, 10147.0, 18662.0, 36523.0, 73031.0, 142515.0, 286149.0, 565179.0, 1020378.0]),
#     'cached_bfs': np.array([3199.0, 3082.0, 3046.0, 3560.0, 6229.0, 13356.0, 23928.0, 41036.0, 94463.0, 182267.0, 372769.0, 723346.0])
# }

# length = 12

# ticks = list(range(1, 1 + length))
# sizes = [(1<<i) for i in range(length)]

# fig, ax = plt.subplots()

# ax.set_title('Speedup of using cached_bfs over bfs')
# # ax.set_title('cached_bfs vs bfs')
# ax.set_xlabel('Batch Size')
# ax.set_xticks(ticks)
# ax.set_xticklabels(sizes)
# ax.set_ylabel('speed-up')
# # ax.set_ylabel('time in microseconds')
# # ax.plot(ticks, runtimes['bfs'], label='bfs')
# # ax.plot(ticks, runtimes['cached_bfs'], label='cached_bfs')
# ax.plot(ticks, runtimes['bfs'] / runtimes['cached_bfs'], label='speed-up')
# ax.legend()
# plt.show()


# runtimes_clients_batch_64 = {
#     'bfs': np.array([38919.0, 38917.0, 42243.0, 41022.0, 38232.0, 37961.0, 40349.0, 38279.0, 40302.0, 41824.0, 42497.0, 41240.0, 40859.0, 41652.0, 39773.0]), 
#     'cached_bfs': np.array([29436.0, 29725.0, 31886.0, 31709.0, 29626.0, 33956.0, 31580.0, 33152.0, 33956.0, 34754.0, 34447.0, 34394.0, 32939.0, 32118.0, 34511.0])
# }


# fig, ax = plt.subplots()

# clients = np.arange(1, 16)

# ax.set_title('response time vs number of clients, batch size = 64')

# ax.set_xlabel('number of clients')
# ax.set_xticks(clients)
# ax.set_xticklabels(clients)

# ax.set_ylabel('runtime in milliseconds')
# ax.plot(clients, runtimes_clients_batch_64['bfs'] / 1000, label='bfs')
# ax.plot(clients, runtimes_clients_batch_64['cached_bfs'] / 1000, label='cached_bfs')

# # ax.plot(clients, runtimes['bfs'] / runtimes['cached_bfs'], label='speed-up')
# ax.legend()
# plt.show()


runtimes_vs_read_ratio = {
    'bfs': np.array([34290.0, 40062.0, 62174.0, 65448.0, 83328.0, 101813.0, 113427.0, 127035.0, 143794.0, 153572.0, 129795.0]),
    'cached_bfs': np.array([36648.0, 45628.0, 71731.0, 76120.0, 94024.0, 91195.0, 103268.0, 97754.0, 85979.0, 79696.0, 50561.0])
}


fig, ax = plt.subplots()

ratios = [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1]

ax.set_title('response time vs read_ratio, batch size = 256')

ax.set_xlabel('read_ratio')
ax.set_xticks(ratios)
ax.set_xticklabels(ratios)

ax.set_ylabel('runtime in milliseconds')
ax.plot(ratios, runtimes_vs_read_ratio['bfs'] / 1000, label='bfs')
ax.plot(ratios, runtimes_vs_read_ratio['cached_bfs'] / 1000, label='cached_bfs')

# ax.plot(clients, runtimes['bfs'] / runtimes['cached_bfs'], label='speed-up')
ax.legend()
plt.show()
