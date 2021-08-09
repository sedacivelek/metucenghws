import numpy as np
import matplotlib.pyplot as plt


def calculate_distances(train_data, test_datum):
    """
    Calculates euclidean distances between test_datum and every train_data
    :param train_data: An (N, D) shaped numpy array where N is the number of examples
    and D is the dimension of the data
    :param test_datum: A (D, ) shaped numpy array
    :return: An (N, ) shaped numpy array that contains distances
    """
    n = train_data.shape[0]
    dist = []
    for i in range(n):
        distance = np.sqrt(np.sum(np.square(train_data[i]-test_datum)))
        dist.append(distance)
    dist = np.asarray(dist)
    return dist


def majority_voting(distances, labels, k):
    """
    Applies majority voting. If there are more then one major class, returns the smallest label.
    :param distances: An (N, ) shaped numpy array that contains distances
    :param labels: An (N, ) shaped numpy array that contains labels
    :param k: An integer. The number of nearest neighbor to be selected.
    :return: An integer. The label of the majority class.
    """
    nearest_index = np.argsort(distances)
    k_neighbor_labels = []
    for i in range(k):
        index = nearest_index[i]
        label = labels[index]
        k_neighbor_labels.append(label)
    major_class = np.argmax(np.bincount(k_neighbor_labels))
    return major_class


def knn(train_data, train_labels, test_data, test_labels, k):
    """
    Calculates accuracy of knn on test data using train_data.
    :param train_data: An (N, D) shaped numpy array where N is the number of examples
    and D is the dimension of the data
    :param train_labels: An (N, ) shaped numpy array that contains labels
    :param test_data: An (M, D) shaped numpy array where M is the number of examples
    and D is the dimension of the data
    :param test_labels: An (M, ) shaped numpy array that contains labels
    :param k: An integer. The number of nearest neighbor to be selected.
    :return: A float. The calculated accuracy.
    """
    pred_labels = []
    for t in test_data:
        dist = calculate_distances(train_data, t)
        pred_class = majority_voting(dist, train_labels, k)
        pred_labels.append(pred_class)
    correct_pred_count = np.sum(pred_labels == test_labels)
    acc = correct_pred_count/len(test_labels)
    return acc


def split_train_and_validation(whole_train_data, whole_train_labels, validation_index, k_fold):
    """
    Splits training dataset into k and returns the validation_indexth one as the
    validation set and others as the training set. You can assume k_fold divides N.
    :param whole_train_data: An (N, D) shaped numpy array where N is the number of examples
    and D is the dimension of the data
    :param whole_train_labels: An (N, ) shaped numpy array that contains labels
    :param validation_index: An integer. 0 <= validation_index < k_fold. Specifies which fold
    will be assigned as validation set.
    :param k_fold: The number of groups that the whole_train_data will be divided into.
    :return: train_data, train_labels, validation_data, validation_labels
    train_data.shape is (N-N/k_fold, D).
    train_labels.shape is (N-N/k_fold, ).
    validation_data.shape is (N/k_fold, D).
    validation_labels.shape is (N/k_fold, ).
    """
    dimension = whole_train_data.shape[1]
    train_data_chunks = np.array_split(whole_train_data, k_fold)
    train_label_chunks = np.array_split(whole_train_labels, k_fold)
    validation_data = train_data_chunks[validation_index]
    validation_labels = train_label_chunks[validation_index]
    train_data = np.delete(train_data_chunks, validation_index, 0)
    train_data = train_data.reshape((-1, dimension))
    train_labels = np.delete(train_label_chunks, validation_index, 0)
    train_labels = train_labels.flatten()
    return train_data, train_labels, validation_data, validation_labels


def cross_validation(whole_train_data, whole_train_labels, k, k_fold):
    """
    Applies k_fold cross-validation and averages the calculated accuracies.
    :param whole_train_data: An (N, D) shaped numpy array where N is the number of examples
    and D is the dimension of the data
    :param whole_train_labels: An (N, ) shaped numpy array that contains labels
    :param k: An integer. The number of nearest neighbor to be selected.
    :param k_fold: An integer.
    :return: A float. Average accuracy calculated.
    """
    accuracies = []
    for i in range(k_fold):
        train_data, train_labels, validation_data, validation_labels = split_train_and_validation(whole_train_data, whole_train_labels, i, k_fold)
        accuracy = knn(train_data, train_labels, validation_data, validation_labels, k)
        accuracies.append(accuracy)
    avg_accuracy = np.mean(accuracies)
    return avg_accuracy

def cross_validation_experiment(train_data, train_labels):
    """
    applies k-fold cross validation for k=1,199
    :param train_data
    :param train_labels
    :return: best k value of the best accuracy
    """
    accuracies = []
    for i in range(1, 200):
        avg = cross_validation(train_data, train_labels, i, 10)
        accuracies.append(avg)
    fig = plt.figure()
    dim = np.arange(1,len(accuracies)+1)
    plt.plot(dim,accuracies, label='Accuracy')
    plt.xlabel('k')
    plt.ylabel('accuracy')
    plt.grid()
    plt.legend()
    plt.tight_layout()
    fig.savefig('knn_cross_validation.png')
    best_k = np.argmax(accuracies)+1
    return best_k


def main():
    train_data = np.load('hw2_data/knn/train_data.npy')
    train_labels = np.load('hw2_data/knn/train_labels.npy')
    test_data = np.load('hw2_data/knn/test_data.npy')
    test_labels = np.load('hw2_data/knn/test_labels.npy')

    best_k = cross_validation_experiment(train_data, train_labels)
    print("Best k value: ", best_k)
    test_accuracy = knn(train_data, train_labels, test_data, test_labels, best_k)
    print("Test accuracy for best k: ", test_accuracy)


if __name__ == '__main__':
    main()