import math


def vocabulary(data):
    """
    Creates the vocabulary from the data.
    :param data: List of lists, every list inside it contains words in that sentence.
                 len(data) is the number of examples in the data.
    :return: Set of words in the data
    """
    vocab = set()
    for d in data:
        for word in d:
            vocab.add(word)
    return vocab


def estimate_pi(train_labels):
    """
    Estimates the probability of every class label that occurs in train_labels.
    :param train_labels: List of class names. len(train_labels) is the number of examples in the training data.
    :return: pi. pi is a dictionary. Its keys are class names and values are their probabilities.
    """
    train_len = len(train_labels)
    pi = {x: train_labels.count(x)/train_len for x in train_labels}
    return pi


def estimate_theta(train_data, train_labels, vocab):
    """
    Estimates the probability of a specific word given class label using additive smoothing with smoothing constant 1.
    :param train_data: List of lists, every list inside it contains words in that sentence.
                       len(train_data) is the number of examples in the training data.
    :param train_labels: List of class names. len(train_labels) is the number of examples in the training data.
    :param vocab: Set of words in the training set.
    :return: theta. theta is a dictionary of dictionaries. At the first level, the keys are the class names. At the
             second level, the keys are all the words in vocab and the values are their estimated probabilities given
             the first level class name.
    """
    d = len(vocab)
    num_classes = set(train_labels)
    label_dict = dict()
    for label in num_classes:
        data_per_label = []
        for i in range(len(train_labels)):
            if train_labels[i] == label:
                data_per_label += train_data[i]
        word_count = len(data_per_label)
        word_prob = dict()
        for word in vocab:
            num_word = data_per_label.count(word)
            prob = (1+num_word)/(d+word_count)
            word_prob[word] = prob
        label_dict[label] = word_prob
    return label_dict


def test(theta, pi, vocab, test_data):
    """
    Calculates the scores of a test data given a class for each class. Skips the words that are not occurring in the
    vocabulary.
    :param theta: A dictionary of dictionaries. At the first level, the keys are the class names. At the second level,
                  the keys are all of the words in vocab and the values are their estimated probabilities.
    :param pi: A dictionary. Its keys are class names and values are their probabilities.
    :param vocab: Set of words in the training set.
    :param test_data: List of lists, every list inside it contains words in that sentence.
                      len(test_data) is the number of examples in the test data.
    :return: scores, list of lists. len(scores) is the number of examples in the test set. Every inner list contains
             tuples where the first element is the score and the second element is the class name.
    """
    scores = []
    for data in test_data:
        score_per_data = []
        for label in pi:
            estimation = 0
            for word in vocab:
                num_count = data.count(word)
                if num_count != 0:
                    estimation += num_count*math.log(theta[label][word])
            estimation += math.log(pi[label])
            score_per_data.append((estimation, label))
        scores.append(score_per_data)
    return scores


if __name__ == '__main__':
    train_file = open("hw4_data/sentiment/train_data.txt", "r")
    train_sentences = [line.rstrip('\n') for line in train_file]
    train_file.close()
    train_data = [sentence.split(' ') for sentence in train_sentences]
    train_label_file = open("hw4_data/sentiment/train_labels.txt", "r")
    train_labels = [line.rstrip('\n') for line in train_label_file]
    train_label_file.close()
    test_file = open("hw4_data/sentiment/test_data.txt", "r")
    test_sentences = [line.rstrip('\n') for line in test_file]
    test_file.close()
    test_data = [sentence.split(' ') for sentence in test_sentences]
    test_label_file = open("hw4_data/sentiment/test_labels.txt", "r")
    test_labels = [line.rstrip('\n') for line in test_label_file]
    test_label_file.close()


    vocab = vocabulary(train_data)
    print("vocab")
    print(vocab)
    pi = estimate_pi(train_labels)
    print("pi")
    print(pi)
    theta = estimate_theta(train_data, train_labels, vocab)
    print("theta")
    print(theta)
    test = test(theta, pi, vocab, test_data)
    print("test")
    print(test)
    test_predict = []
    for t in test:
        test_predict.append(max(t, key=lambda score: score[0])[1])
    acc = 0
    for i in range(len(test_labels)):
        if test_predict[i] == test_labels[i]:
            acc += 1
    print("The test accuracy is:", acc/len(test_labels))
