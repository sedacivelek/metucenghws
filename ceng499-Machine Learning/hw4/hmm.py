import numpy as np


def forward(A, B, pi, O):
    """
    Calculates the probability of an observation sequence O given the model(A, B, pi).
    :param A: state transition probabilities (NxN)
    :param B: observation probabilites (NxM)
    :param pi: initial state probabilities (N)
    :param O: sequence of observations(T) where observations are just indices for the columns of B (0-indexed)
        N is the number of states,
        M is the number of possible observations, and
        T is the sequence length.
    :return: The probability of the observation sequence and the calculated alphas in the Trellis diagram with shape
             (N, T) which should be a numpy array.
    """
    alpha = np.zeros((A.shape[0], O.shape[0]))
    alpha[:, 0] = B[:, O[0]]*pi
    for i in range(1,  O.shape[0]):
        for j in range(A.shape[0]):
            observed = O[i]
            prev_alpha = alpha[:, i - 1]
            node_alpha = (A[:, j]) * B[j, observed] * prev_alpha
            alpha_sum = 0
            for n in node_alpha:
                alpha_sum += n
            alpha[j, i] = alpha_sum
    result = sum(last[-1] for last in alpha)
    return result, alpha


def viterbi(A, B, pi, O):
    """
    Calculates the most likely state sequence given model(A, B, pi) and observation sequence.
    :param A: state transition probabilities (NxN)
    :param B: observation probabilites (NxM)
    :param pi: initial state probabilities(N)
    :param O: sequence of observations(T) where observations are just indices for the columns of B (0-indexed)
        N is the number of states,
        M is the number of possible observations, and
        T is the sequence length.
    :return: The most likely state sequence with shape (T,) and the calculated deltas in the Trellis diagram with shape
             (N, T). They should be numpy arrays.
    """

    delta = np.zeros((A.shape[0], O.shape[0]))
    delta[:, 0] = B[:, O[0]]*pi
    trace_back = np.zeros((A.shape[0], O.shape[0]))
    most_likely_path = []
    for i in range(1, O.shape[0]):
        for j in range(A.shape[0]):
            observed = O[i]
            prev_delta = delta[:, i - 1]
            node_delta = (A[:, j]) * B[j, observed] * prev_delta
            max_node = float('-inf')
            max_index = -1
            for n in range(len(node_delta)):
                if node_delta[n]> max_node:
                    max_node = node_delta[n]
                    max_index = n
            trace_back[j, i] = max_index
            delta[j, i] = max_node
    last_node = delta[:, O.shape[0] - 1]
    last_node_max = float('-inf')
    last_node_path_index = -1
    for l in range(len(last_node)):
        if last_node[l]>last_node_max:
            last_node_max = last_node[l]
            last_node_path_index = l

    trace_edge = last_node_path_index
    most_likely_path.append(int(trace_edge))

    for i in range(O.shape[0] - 1, 0, -1):
        most_likely_path.append(trace_back[int(trace_edge), i])
        trace_edge = trace_back[int(trace_edge), i]
    return np.array(most_likely_path[::-1]),delta
