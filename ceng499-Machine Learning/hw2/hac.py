import numpy as np
import matplotlib.pyplot as plt

def single_linkage(c1, c2):
    """
    Given clusters c1 and c2, calculates the single linkage criterion.
    :param c1: An (N, D) shaped numpy array containing the data points in cluster c1.
    :param c2: An (M, D) shaped numpy array containing the data points in cluster c2.
    :return: A float. The result of the calculation.
    """
    distance_matrix = np.zeros((len(c1), len(c2)))
    for i in range(len(c1)):
        for j in range(len(c2)):
            distance_matrix[i, j] = euclidean_distance(c1[i], c2[j])
    return np.min(distance_matrix)


def complete_linkage(c1, c2):
    """
    Given clusters c1 and c2, calculates the complete linkage criterion.
    :param c1: An (N, D) shaped numpy array containing the data points in cluster c1.
    :param c2: An (M, D) shaped numpy array containing the data points in cluster c2.
    :return: A float. The result of the calculation.
    """
    distance_matrix = np.empty((len(c1), len(c2)))
    for i in range(len(c1)):
        for j in range(len(c2)):
            distance_matrix[i, j] = euclidean_distance(c1[i], c2[j])
    return np.max(distance_matrix)


def average_linkage(c1, c2):
    """
    Given clusters c1 and c2, calculates the average linkage criterion.
    :param c1: An (N, D) shaped numpy array containing the data points in cluster c1.
    :param c2: An (M, D) shaped numpy array containing the data points in cluster c2.
    :return: A float. The result of the calculation.
    """
    distance_matrix = np.empty((len(c1), len(c2)))
    for i in range(len(c1)):
        for j in range(len(c2)):
            distance_matrix[i, j] = euclidean_distance(c1[i], c2[j])
    return np.mean(distance_matrix)


def centroid_linkage(c1, c2):
    """
    Given clusters c1 and c2, calculates the centroid linkage criterion.
    :param c1: An (N, D) shaped numpy array containing the data points in cluster c1.
    :param c2: An (M, D) shaped numpy array containing the data points in cluster c2.
    :return: A float. The result of the calculation.
    """
    centroid1 = np.mean(c1, axis=0)
    centroid2 = np.mean(c2, axis=0)
    return euclidean_distance(centroid1, centroid2)


def hac(data, criterion, stop_length):
    """
    Applies hierarchical agglomerative clustering algorithm with the given criterion on the data
    until the number of clusters reaches the stop_length.
    :param data: An (N, D) shaped numpy array containing all of the data points.
    :param criterion: A function. It can be single_linkage, complete_linkage, average_linkage, or
    centroid_linkage
    :param stop_length: An integer. The length at which the algorithm stops.
    :return: A list of numpy arrays with length stop_length. Each item in the list is a cluster
    and a (Ni, D) sized numpy array.
    """
    k_clusters = []
    init_clusters = data
    init_clusters = np.expand_dims(init_clusters,axis=1)
    init_clusters = init_clusters.tolist()
    while len(init_clusters) > stop_length:
        nearest = float('inf')
        merger= -1
        mergec= -1
        for r in range(len(init_clusters)):
            for c in range(len(init_clusters)):
                if r!=c:
                    dist = criterion(np.array(init_clusters[r]),np.array(init_clusters[c]))
                    if dist<nearest:
                        nearest = dist
                        merger = r
                        remover = init_clusters[merger]
                        mergec = c
                        removec = init_clusters[mergec]
        new_cluster = np.vstack([init_clusters[merger],init_clusters[mergec]])
        new_cluster = new_cluster.tolist()
        init_clusters.remove(remover)
        init_clusters.remove(removec)
        init_clusters.append(new_cluster)
    for nc in init_clusters:
        k_clusters.append(np.asarray(nc))
    return k_clusters


def euclidean_distance(point1, point2):
    return np.sqrt(np.sum(np.square(point1 - point2)))


def main():
    data1 = np.load('hw2_data/hac/data1.npy')
    data2 = np.load('hw2_data/hac/data2.npy')
    data3 = np.load('hw2_data/hac/data3.npy')
    data4 = np.load('hw2_data/hac/data4.npy')

    criterions = [single_linkage,complete_linkage,average_linkage,centroid_linkage]
    criterions_name = ["single_linkage","complete_linkage","average_linkage","centroid_linkage"]
    i =0
    for c in criterions:
        clusters1 = hac(data1,c,2)
        cluster10 = clusters1[0]
        cluster11 = clusters1[1]
        fig = plt.figure()
        plt.plot(cluster10[:, 0], cluster10[:, 1], 'r.')
        plt.plot(cluster11[:, 0], cluster11[:, 1], 'k.')
        fig.savefig('hac_clustering1_'+criterions_name[i]+'.png')
        plt.clf()
        i+=1
    i = 0
    for c in criterions:
        clusters2 = hac(data2, c, 2)
        cluster20 = clusters2[0]
        cluster21 = clusters2[1]
        fig = plt.figure()
        plt.plot(cluster20[:, 0], cluster20[:, 1], 'r.')
        plt.plot(cluster21[:, 0], cluster21[:, 1], 'k.')
        fig.savefig('hac_clustering2_' + criterions_name[i] + '.png')
        plt.clf()
        i += 1
    i = 0
    for c in criterions:
        clusters3 = hac(data3, c, 2)
        cluster30 = clusters3[0]
        cluster31 = clusters3[1]
        fig = plt.figure()
        plt.plot(cluster30[:, 0], cluster30[:, 1], 'r.')
        plt.plot(cluster31[:, 0], cluster31[:, 1], 'k.')
        fig.savefig('hac_clustering3_' + criterions_name[i] + '.png')
        plt.clf()
        i += 1
    i = 0
    for c in criterions:
        clusters4 = hac(data4, c, 4)
        cluster40 = clusters4[0]
        cluster41 = clusters4[1]
        cluster42 = clusters4[2]
        cluster43 = clusters4[3]
        fig = plt.figure()
        plt.plot(cluster40[:, 0], cluster40[:, 1], 'r.')
        plt.plot(cluster41[:, 0], cluster41[:, 1], 'k.')
        plt.plot(cluster42[:, 0], cluster42[:, 1], 'm.')
        plt.plot(cluster43[:, 0], cluster43[:, 1], 'g.')
        fig.savefig('hac_clustering4_' + criterions_name[i] + '.png')
        plt.clf()
        i += 1
if __name__ == '__main__':
    main()

