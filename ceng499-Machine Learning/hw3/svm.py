import numpy as np
from sklearn.svm import SVC
from sklearn.model_selection import GridSearchCV
from draw import draw_svm
from sklearn.metrics import confusion_matrix

def part1():
    train_data = np.load('hw3_data/linsep/train_data.npy')
    train_labels = np.load('hw3_data/linsep/train_labels.npy')

    c_values = [0.01, 0.1, 1, 10, 100]

    for c in c_values:
        clf = SVC(C=c, kernel='linear')
        clf.fit(train_data, train_labels)
        draw_svm(clf, train_data, train_labels, -3, 3, -3, 3, target='part1_'+str(c)+".png")


def part2():
    train_data = np.load('hw3_data/nonlinsep/train_data.npy')
    train_labels = np.load('hw3_data/nonlinsep/train_labels.npy')

    kernels = ["linear", "rbf", "poly", "sigmoid"]

    for k in kernels:
        clf = SVC(C=1, kernel=k)
        clf.fit(train_data, train_labels)
        draw_svm(clf, train_data, train_labels, -3, 3, -3, 3, target='part2_'+str(k)+".png")

def part3():
    train_data = np.load('hw3_data/fashion_mnist/train_data.npy')
    train_labels = np.load('hw3_data/fashion_mnist/train_labels.npy')
    test_data = np.load('hw3_data/fashion_mnist/test_data.npy')
    test_labels = np.load('hw3_data/fashion_mnist/test_labels.npy')
    train_data = train_data/256
    test_data = test_data/256


    grid_params = {
        'C': [0.01, 0.1, 1, 10, 100],
        'gamma': [0.00001, 0.0001, 0.001, 0.01, 0.1, 1],
        'kernel': ['rbf', 'poly', 'sigmoid','linear']
    }

    x_train_data = train_data.reshape(train_data.shape[0],train_data.shape[1]*train_data.shape[2])
    x_test_data = test_data.reshape(test_data.shape[0], test_data.shape[1] * test_data.shape[2])
    grid = GridSearchCV(estimator=SVC(), param_grid=grid_params)
    grid.fit(x_train_data,train_labels)
    means =grid.cv_results_['mean_test_score']
    print("Grid search accuracy values for each parameters")
    for mean, params in zip(means, grid.cv_results_['params']):
        print("%0.3f for %r" % (mean, params))
    best_param = grid.best_params_
    print("Best parameters are: ", best_param)
    print("Test accuracy with best hyperparameters: ",grid.score(x_test_data,test_labels))

def part4():
    train_data = np.load('hw3_data/fashion_mnist_imba/train_data.npy')
    train_labels = np.load('hw3_data/fashion_mnist_imba/train_labels.npy')
    test_data = np.load('hw3_data/fashion_mnist_imba/test_data.npy')
    test_labels = np.load('hw3_data/fashion_mnist_imba/test_labels.npy')
    train_data = train_data / 256
    test_data = test_data / 256
    x_train_data = train_data.reshape(train_data.shape[0], train_data.shape[1] * train_data.shape[2])
    x_test_data = test_data.reshape(test_data.shape[0], test_data.shape[1] * test_data.shape[2])

    clf = SVC(C=1, kernel='rbf')
    clf.fit(x_train_data,train_labels)
    tn, fp, fn, tp = confusion_matrix(test_labels,clf.predict(x_test_data)).ravel()
    print("Confusion matrix")
    print("TN:",tn, " FP:", fp, " FN:",fn," TP:",tp)
    print("Test accuracy of imbalanced dataset: ", clf.score(x_test_data, test_labels))
    print("Recall of imbalanced dataset: ", tp / (tp + fn))
    print("Precision of imbalanced dataset: ", tp / (tp + fp))
    print("----------------------------------------")
    class_0 = np.count_nonzero(train_labels == 0)
    class_1 = np.count_nonzero(train_labels == 1)

    if class_0 < class_1:
        minority = 0
        majority = 1
        oversamp = int(class_1/class_0)
        undersamp = class_0
        undersamp_maj = class_1
    else:
        minority = 1
        majority = 0
        oversamp = int(class_0/class_1)
        undersamp = class_1
        undersamp_maj = class_0

    # Oversampling

    oversampled_train_data = []
    for d in train_data:
        oversampled_train_data.append(d)

    oversampled_train_labels = []
    for t in train_labels:
        oversampled_train_labels.append(t)
    for l in range(oversamp):
        for d in range(len(train_data)):
            if train_labels[d] == minority:
                oversampled_train_data.append(train_data[d])
                oversampled_train_labels.append(train_labels[d])
    oversampled_train_data = np.array(oversampled_train_data)
    oversampled_train_labels = np.array(oversampled_train_labels)
    x_oversampled_train_data = oversampled_train_data.reshape(oversampled_train_data.shape[0], oversampled_train_data.shape[1] * oversampled_train_data.shape[2])
    ovclf = SVC(C=1, kernel='rbf')
    ovclf.fit(x_oversampled_train_data,oversampled_train_labels)
    tn, fp, fn, tp = confusion_matrix(test_labels, ovclf.predict(x_test_data)).ravel()
    print("Confusion matrix for oversampled dataset")
    print("TN:", tn, " FP:", fp, " FN:", fn, " TP:", tp)
    print("Test accuracy of oversampled dataset: ", ovclf.score(x_test_data, test_labels))
    print("Recall of oversampled dataset: ",tp/(tp+fn))
    print("Precision of oversampled dataset: ", tp/(tp+fp))
    print("----------------------------------------")

    # Undersampling

    undersampled_train_data = train_data
    undersampled_train_labels = train_labels
    while undersamp_maj>undersamp:
        pos = np.argmax(undersampled_train_labels==majority)
        undersampled_train_data = np.delete(undersampled_train_data,pos,axis=0)
        undersampled_train_labels = np.delete(undersampled_train_labels,pos)
        undersamp_maj-=1
    x_undersampled_train_data = undersampled_train_data.reshape(undersampled_train_data.shape[0], undersampled_train_data.shape[1] * undersampled_train_data.shape[2])
    unclf = SVC(C=1, kernel='rbf')
    unclf.fit(x_undersampled_train_data, undersampled_train_labels)
    tn, fp, fn, tp = confusion_matrix(test_labels, unclf.predict(x_test_data)).ravel()
    print("Confusion matrix for undersampled dataset")
    print("TN:", tn, " FP:", fp, " FN:", fn, " TP:", tp)
    print("Test accuracy of undersampled dataset: ", unclf.score(x_test_data, test_labels))
    print("Recall of undersampled dataset: ", tp / (tp + fn))
    print("Precision of undersampled dataset: ", tp / (tp + fp))
    print("----------------------------------------")

    # Make balanced

    x_balanced_data = train_data.reshape(train_data.shape[0], train_data.shape[1] * train_data.shape[2])
    balancedclf = SVC(C=1, kernel='rbf',class_weight='balanced')
    balancedclf.fit(x_balanced_data, train_labels)
    tn, fp, fn, tp = confusion_matrix(test_labels, balancedclf.predict(x_test_data)).ravel()
    print("Confusion matrix for balanced dataset")
    print("TN:", tn, " FP:", fp, " FN:", fn, " TP:", tp)
    print("Test accuracy of balanced dataset: ", balancedclf.score(x_test_data, test_labels))
    print("Recall of balanced dataset: ", tp / (tp + fn))
    print("Precision of balanced dataset: ", tp / (tp + fp))


if __name__ == '__main__':
    #part1()
    #part2()
    part3()
    #part4()
