import numpy as np
import matplotlib.pyplot as plt


def assign_clusters(data, cluster_centers):
    """
    Assigns every data point to its closest (in terms of euclidean distance) cluster center.
    :param data: An (N, D) shaped numpy array where N is the number of examples
    and D is the dimension of the data
    :param cluster_centers: A (K, D) shaped numpy array where K is the number of clusters
    and D is the dimension of the data
    :return: An (N, ) shaped numpy array. At its index i, the index of the closest center
    resides to the ith data point.
    """
    assign = []
    for d in data:
        dist = []
        for c in cluster_centers:
            distance = np.sqrt(np.sum(np.square(d-c)))
            dist.append(distance)
        assign.append(np.argmin(dist))
    assign = np.asarray(assign)
    return assign


def calculate_cluster_centers(data, assignments, cluster_centers, k):
    """
    Calculates cluster_centers such that their squared euclidean distance to the data assigned to
    them will be lowest.
    If none of the data points belongs to some cluster center, then assign it to its previous value.
    :param data: An (N, D) shaped numpy array where N is the number of examples
    and D is the dimension of the data
    :param assignments: An (N, ) shaped numpy array with integers inside. They represent the cluster index
    every data assigned to.
    :param cluster_centers: A (K, D) shaped numpy array where K is the number of clusters
    and D is the dimension of the data
    :param k: Number of clusters
    :return: A (K, D) shaped numpy array that contains the newly calculated cluster centers.
    """
    new_cluster_centers = []
    for i in range(k):
        assigned = data[assignments == i]
        if len(assigned) != 0:
            new_center = assigned.mean(axis=0)
            new_cluster_centers.append(new_center)
        else:
            new_cluster_centers.append(cluster_centers[i])
    return np.asarray(new_cluster_centers)


def kmeans(data, initial_cluster_centers):
    """
    Applies k-means algorithm.
    :param data: An (N, D) shaped numpy array where N is the number of examples
    and D is the dimension of the data
    :param initial_cluster_centers: A (K, D) shaped numpy array where K is the number of clusters
    and D is the dimension of the data
    :return: cluster_centers, objective_function
    cluster_center.shape is (K, D).
    objective function is a float. It is calculated by summing the squared euclidean distance between
    data points and their cluster centers.
    """
    prev_centers = np.asarray(initial_cluster_centers)
    while True:
        assignments = assign_clusters(data, prev_centers)
        new_centers = calculate_cluster_centers(data, assignments, prev_centers, initial_cluster_centers.shape[0])
        difference = np.sqrt(np.sum(np.square(new_centers-prev_centers)))
        prev_centers = new_centers
        if difference < 0.0001:
            break
    assignments = assign_clusters(data, new_centers)
    obj_func = calculate_objective_function(data, assignments, new_centers)
    return new_centers, obj_func


def calculate_objective_function(data, assignments, cluster_centers):
    """
    helper function used in kmeans function
    :param data: given data points
    :param assignments: data points that are assigned to the closest center
    :param cluster_centers: center points of the clusters
    :return: sum of the squared euclidean distance between data points and their cluster centers
    """
    distances = []
    for i in range(len(cluster_centers)):
        points = data[assignments == i]
        distance = np.sum(np.square(points-cluster_centers[i]))
        distances.append(distance)
    obj_func = np.sum(distances)
    return obj_func


def initialize_cluster_centers(data, k):
    """
    picks random k points from given data points
    :param data: given data points
    :param k: number of clusters
    :return: cluster centers
    """
    return data[np.random.randint(data.shape[0], size=k)]


def elbow_method(data):
    """
    applies elbow method for k=1,10, restart for each k 10 times, pick the best objective function
    :param data: given data points
    :return: cluster centers and objective function value for each k value
    """
    obj_list = []
    best_centers_list = []
    for k in range(1,11):
        best_obj = float('inf')
        best_center = []
        for restart in range(10):
            init_centers = initialize_cluster_centers(data,k)
            cluster_centers, objective_function = kmeans(data,init_centers)
            if objective_function < best_obj:
                best_obj = objective_function
                best_center = cluster_centers
        best_centers_list.append(best_center)
        obj_list.append(best_obj)
    return best_centers_list,obj_list


def main():
    #Data loading
    clustering1 = np.load('hw2_data/kmeans/clustering1.npy')
    clustering2 = np.load('hw2_data/kmeans/clustering2.npy')
    clustering3 = np.load('hw2_data/kmeans/clustering3.npy')
    clustering4 = np.load('hw2_data/kmeans/clustering4.npy')

    #plot elbow method for clustering1, save png as 'Objective_Function1.png'
    best_centers1, best_obj1 = elbow_method(clustering1)
    best_fig = plt.figure()
    dim = np.arange(1, len(best_obj1) + 1)
    plt.plot(dim, best_obj1)
    plt.xlabel('k')
    plt.ylabel('Objective Function')
    plt.xticks(dim)
    plt.grid()
    best_fig.savefig('Objective_Function1.png')
    #plot resultant clusters for the data clustering1
    assigned_data1 = assign_clusters(clustering1,best_centers1[1])
    cluster10 = clustering1[assigned_data1 == 0]
    cluster11 = clustering1[assigned_data1 == 1]
    plt.clf()
    fig = plt.figure()
    plt.plot(cluster10[:, 0], cluster10[:, 1], 'r.')
    plt.plot(cluster11[:, 0], cluster11[:, 1], 'k.')
    plt.plot(best_centers1[1][:,0], best_centers1[1][:,1], 'ys')
    fig.savefig('kmeans_clustering1.png')

    #plot elbow method for clustering2, save png as 'Objective_Function2.png'
    best_centers2, best_obj2 = elbow_method(clustering2)
    plt.clf()
    best_fig2 = plt.figure()
    dim = np.arange(1, len(best_obj2) + 1)
    plt.plot(dim, best_obj2)
    plt.xlabel('k')
    plt.ylabel('Objective Function')
    plt.xticks(dim)
    plt.grid()
    best_fig2.savefig('Objective_Function2.png')
    #plot resultant clusters for the data clustering2
    assigned_data2 = assign_clusters(clustering2, best_centers2[2])
    cluster20 = clustering2[assigned_data2 == 0]
    cluster21 = clustering2[assigned_data2 == 1]
    cluster22 = clustering2[assigned_data2 == 2]
    plt.clf()
    fig2 = plt.figure()
    plt.plot(cluster20[:, 0], cluster20[:, 1], 'r.')
    plt.plot(cluster21[:, 0], cluster21[:, 1], 'k.')
    plt.plot(cluster22[:, 0], cluster22[:, 1], 'm.')
    plt.plot(best_centers2[2][:,0], best_centers2[2][:,1], 'ys')
    fig2.savefig('kmeans_clustering2.png')

    #plot elbow method for clustering3, save png as 'Objective_Function3.png'
    best_centers3, best_obj3 = elbow_method(clustering3)
    plt.clf()
    best_fig3 = plt.figure()
    dim = np.arange(1, len(best_obj3) + 1)
    plt.plot(dim, best_obj3)
    plt.xlabel('k')
    plt.ylabel('Objective Function')
    plt.xticks(dim)
    plt.grid()
    best_fig3.savefig('Objective_Function3.png')
    #plot resultant clusters for the data clustering3
    assigned_data3 = assign_clusters(clustering3, best_centers3[3])
    cluster30 = clustering3[assigned_data3 == 0]
    cluster31 = clustering3[assigned_data3 == 1]
    cluster32 = clustering3[assigned_data3 == 2]
    cluster33 = clustering3[assigned_data3 == 3]
    plt.clf()
    fig3 = plt.figure()
    plt.plot(cluster30[:, 0], cluster30[:, 1], 'r.')
    plt.plot(cluster31[:, 0], cluster31[:, 1], 'k.')
    plt.plot(cluster32[:, 0], cluster32[:, 1], 'm.')
    plt.plot(cluster33[:, 0], cluster33[:, 1], 'g.')
    plt.plot(best_centers3[3][:, 0], best_centers3[3][:, 1], 'ys')
    fig3.savefig('kmeans_clustering3.png')

    #plot elbow method for clustering4, save png as 'Objective_Function4.png'
    best_centers4, best_obj4 = elbow_method(clustering4)
    plt.clf()
    best_fig4 = plt.figure()
    dim = np.arange(1, len(best_obj4) + 1)
    plt.plot(dim, best_obj4)
    plt.xlabel('k')
    plt.ylabel('Objective Function')
    plt.xticks(dim)
    plt.grid()
    best_fig4.savefig('Objective_Function4.png')
    #plot resultant clusters for the data clustering4
    assigned_data4 = assign_clusters(clustering4, best_centers4[4])
    cluster40 = clustering4[assigned_data4 == 0]
    cluster41 = clustering4[assigned_data4 == 1]
    cluster42 = clustering4[assigned_data4 == 2]
    cluster43 = clustering4[assigned_data4 == 3]
    cluster44 = clustering4[assigned_data4 == 4]
    plt.clf()
    fig4 = plt.figure()
    plt.plot(cluster40[:, 0], cluster40[:, 1], 'r.')
    plt.plot(cluster41[:, 0], cluster41[:, 1], 'k.')
    plt.plot(cluster42[:, 0], cluster42[:, 1], 'm.')
    plt.plot(cluster43[:, 0], cluster43[:, 1], 'g.')
    plt.plot(cluster44[:, 0], cluster44[:, 1], 'c.')
    plt.plot(best_centers4[4][:,0], best_centers4[4][:,1], 'ys')
    fig4.savefig('kmeans_clustering4.png')


if __name__ == '__main__':
    main()
