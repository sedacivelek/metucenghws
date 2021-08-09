import numpy as np
import math
import graphviz

chi_val_table = {
    1: 2.71,
    2: 4.61
}


def entropy(bucket):
    """
    Calculates the entropy.
    :param bucket: A list of size num_classes. bucket[i] is the number of
    examples that belong to class i.
    :return: A float. Calculated entropy.
    """
    example_size = np.sum(bucket)
    ent = 0
    for b in bucket:
        if b != 0:
            ent += -(b / example_size) * (math.log2(b / example_size))
    return ent


def avg_entropy(bucket, ent):
    example_size = np.sum(bucket)
    avg = 0
    for b in bucket:
        avg += (b / example_size) * ent
    return avg


def info_gain(parent_bucket, left_bucket, right_bucket):
    """
    Calculates the information gain. A bucket is a list of size num_classes.
    bucket[i] is the number of examples that belong to class i.
    :param parent_bucket: Bucket belonging to the parent node. It contains the
    number of examples that belong to each class before the split.
    :param left_bucket: Bucket belonging to the left child after the split.
    :param right_bucket: Bucket belonging to the right child after the split.
    :return: A float. Calculated information gain.
    """
    parent_ent = entropy(parent_bucket)
    left_ent = entropy(left_bucket)
    right_ent = entropy(right_bucket)
    example_size = np.sum(parent_bucket)
    gain = parent_ent - (np.sum(left_bucket) / example_size) * left_ent - (
            np.sum(right_bucket) / example_size) * right_ent
    return gain


def gini(bucket):
    """
    Calculates the gini index.
    :param bucket: A list of size num_classes. bucket[i] is the number of
    examples that belong to class i.
    :return: A float. Calculated gini index.
    """
    gini_index = 1
    example_size = np.sum(bucket)
    if example_size != 0:
        for b in bucket:
            gini_index -= ((b / example_size) ** 2)
    return gini_index


def avg_gini_index(left_bucket, right_bucket):
    """
    Calculates the average gini index. A bucket is a list of size num_classes.
    bucket[i] is the number of examples that belong to class i.
    :param left_bucket: Bucket belonging to the left child after the split.
    :param right_bucket: Bucket belonging to the right child after the split.
    :return: A float. Calculated average gini index.
    """
    left_size = np.sum(left_bucket)
    right_size = np.sum(right_bucket)
    example_size = left_size + right_size
    avg_gini = ((left_size / example_size) * gini(left_bucket)) + ((right_size / example_size) * gini(right_bucket))
    return avg_gini


def calculate_split_values(data, labels, num_classes, attr_index, heuristic_name):
    """
    For every possible values to split the data for the attribute indexed by
    attribute_index, it divides the data into buckets and calculates the values
    returned by the heuristic function named heuristic_name. The split values
    should be the average of the closest 2 values. For example, if the data has
    2.1 and 2.2 in it consecutively for the values of attribute index by attr_index,
    then one of the split values should be 2.15.
    :param data: An (N, M) shaped numpy array. N is the number of examples in the
    current node. M is the dimensionality of the data. It contains the values for
    every attribute for every example.
    :param labels: An (N, ) shaped numpy array. It contains the class values in
    it. For every value, 0 <= value < num_classes.
    :param num_classes: An integer. The number of classes in the dataset.
    :param attr_index: An integer. The index of the attribute that is going to
    be used for the splitting operation. This integer indexes the second dimension
    of the data numpy array.
    :param heuristic_name: The name of the heuristic function. It should either be
    'info_gain' of 'avg_gini_index' for this homework.
    :return: An (L, 2) shaped numpy array. L is the number of split values. The
    first column is the split values and the second column contains the calculated
    heuristic values for their splits.
    """

    att_values = data[:, attr_index]
    att_values = np.sort(att_values)
    result = []
    for i in range(len(att_values) - 1):
        left_bucket = []
        left_labels = []
        right_bucket = []
        right_labels = []
        avg_val = np.average(att_values[i:i + 2])
        for j in range(len(data)):
            if data[j, attr_index] < avg_val:
                left_bucket.append(data[j])
                left_labels.append(labels[j])
            else:
                right_bucket.append(data[j])
                right_labels.append(labels[j])
        parent_examples = build_bucket(labels, num_classes)
        left_examples = build_bucket(left_labels, num_classes)
        right_examples = build_bucket(right_labels, num_classes)

        if heuristic_name == 'info_gain':
            result.append([avg_val, info_gain(parent_examples, left_examples, right_examples)])
        else:
            result.append([avg_val, avg_gini_index(left_examples, right_examples)])
    return result


def chi_squared_test(left_bucket, right_bucket):
    """
    Calculates chi squared value and degree of freedom between the selected attribute
    and the class attribute. A bucket is a list of size num_classes. bucket[i] is the
    number of examples that belong to class i.
    :param left_bucket: Bucket belonging to the left child after the split.
    :param right_bucket: Bucket belonging to the right child after the split.
    :return: A float and and integer. Chi squared value and degree of freedom.
    """
    all_ex = np.sum(left_bucket) + np.sum(right_bucket)
    num_class = len(left_bucket)
    chi_val = 0
    deleted = 0
    for i in range(num_class):
        if left_bucket[i] == 0 and right_bucket[i] == 0:
            deleted += 1
        else:
            exp_l = (left_bucket[i] + right_bucket[i]) * np.sum(left_bucket) / all_ex
            exp_r = (left_bucket[i] + right_bucket[i]) * np.sum(right_bucket) / all_ex
            chi_val += ((left_bucket[i] - exp_l) ** 2) / exp_l
            chi_val += ((right_bucket[i] - exp_r) ** 2) / exp_r
    df = num_class - deleted - 1
    return chi_val, df


def best_attribute(data, labels, heuristic_name):
    best_att = -1
    best_idx = -1
    best_result = []
    if heuristic_name == 'info_gain':
        best = 0
        for att_index in range(4):
            result = np.array(calculate_split_values(data, labels, 3, att_index, heuristic_name))
            idx = np.argmax(result[:, 1])
            if result[idx, 1] > best:
                best = result[idx, 1]
                best_att = att_index
                best_idx = idx
                best_result = result
    else:
        best = float('inf')
        for att_index in range(4):
            result = np.array(calculate_split_values(data, labels, 3, att_index, heuristic_name))

            idx = np.argmin(result[:, 1])

            if result[idx, 1] < best:
                best = result[idx, 1]
                best_att = att_index
                best_idx = idx
                best_result = result
    return best_att, best_result[best_idx]


class Node:
    def __init__(self):
        self.attribute = None
        self.value = None
        self.data = None
        self.label = None
        self.right = None
        self.left = None
        self.decision = None
        self.bucket = None


def grow_tree(data, label, heuristic_name, pre_prune):
    ent = entropy(build_bucket(label, 3))
    node = Node()
    node.data = data
    node.label = label
    node.bucket = build_bucket(label, 3)
    if ent != 0:
        best_att, best_att_val = best_attribute(data, label, heuristic_name)
        left_data, left_labels, right_data, right_labels = split_nodes(data, label, best_att, best_att_val)
        chi_val, df = chi_squared_test(build_bucket(left_labels, 3), build_bucket(right_labels, 3))
        if pre_prune:
            if not chi_val > chi_val_table[df]:
                node.decision = np.argmax(build_bucket(label, 3))
                node.bucket = build_bucket(label, 3)
                return node
        node.attribute = best_att
        node.value = best_att_val[0]
        node.left = grow_tree(left_data, left_labels, heuristic_name, pre_prune)
        node.right = grow_tree(right_data, right_labels, heuristic_name, pre_prune)
    else:
        node.decision = np.argmax(build_bucket(label, 3))
        node.bucket = build_bucket(label, 3)
    return node


def build_bucket(label, num_classes):
    res = []
    for l in range(num_classes):
        res.append(np.count_nonzero(np.array(label) == l))
    return res


def split_nodes(train_data, train_labels, node_att, node_vals):
    left_data = []
    left_labels = []
    right_data = []
    right_labels = []
    for i in range(len(train_labels)):
        if train_data[i, node_att] < node_vals[0]:
            left_data.append(train_data[i])
            left_labels.append(train_labels[i])
        else:
            right_data.append(train_data[i])
            right_labels.append(train_labels[i])
    return np.array(left_data), np.array(left_labels), np.array(right_data), np.array(right_labels)


def predict(data, root):
    if root.value is not None:
        if data[root.attribute] < root.value:
            return predict(data, root.left)
        else:
            return predict(data, root.right)
    return root.decision


def draw_tree(root, path):
    def draw_node(tree, dot=None):
        if dot is None:
            dot = graphviz.Digraph(comment='Decision Tree')
            dot.node(name=str(tree),
                     label="x[" + str(tree.attribute) + "]<" + str(tree.value) + "\n" + str(tree.bucket))
        if tree.left:
            if tree.left.value:
                dot.node(name=str(tree.left),
                         label="x[" + str(tree.left.attribute) + "]<" + str(tree.left.value) + "\n" + str(
                             tree.left.bucket))

            else:
                dot.node(name=str(tree.left),
                         label=str(tree.left.bucket))

            dot.edge(str(tree), str(tree.left))
            dot = draw_node(tree.left, dot=dot)
        if tree.right:
            if tree.right.value:
                dot.node(name=str(tree.right),
                         label="x[" + str(tree.right.attribute) + "]<" + str(tree.right.value) + "\n" + str(
                             tree.right.bucket))
            else:
                dot.node(name=str(tree.right),
                         label=str(tree.right.bucket))

            dot.edge(str(tree), str(tree.right))
            dot = draw_node(tree.right, dot=dot)
        return dot

    dot = draw_node(root)
    dot.render(path, format="png", view=False)


if __name__ == '__main__':
    train_data = np.load('hw3_data/iris/train_data.npy')
    train_labels = np.load('hw3_data/iris/train_labels.npy')
    test_data = np.load('hw3_data/iris/test_data.npy')
    test_labels = np.load('hw3_data/iris/test_labels.npy')

    # Info gain experiment

    tree = grow_tree(train_data, train_labels, 'info_gain', pre_prune=False)
    acc = 0
    for d in range(len(test_data)):
        prediction = predict(test_data[d], tree)
        reel = test_labels[d]
        if predict(test_data[d], tree) == test_labels[d]:
            acc += 1

    print("Gain accuracy: ", acc / len(test_data))
    draw_tree(tree, 'info_gain_tree')

    # Avg gini index experiment

    tree = grow_tree(train_data, train_labels, 'avg_gini_index', pre_prune=False)
    acc = 0
    for d in range(len(test_data)):
        prediction = predict(test_data[d], tree)
        reel = test_labels[d]
        if predict(test_data[d], tree) == test_labels[d]:
            acc += 1
    print("Gini accuracy: ", acc / len(test_data))
    draw_tree(tree, 'avg_gini_tree')

    # Info gain experiment with pruning

    tree = grow_tree(train_data, train_labels, 'info_gain', pre_prune=True)
    acc = 0
    for d in range(len(test_data)):
        prediction = predict(test_data[d], tree)
        reel = test_labels[d]
        if predict(test_data[d], tree) == test_labels[d]:
            acc += 1

    print("Gain accuracy with pre-prune: ", acc / len(test_data))
    draw_tree(tree, 'info_gain_prune_tree')

    # Avg gini index experiment with pruning

    tree = grow_tree(train_data, train_labels, 'avg_gini_index', pre_prune=True)
    acc = 0
    for d in range(len(test_data)):
        prediction = predict(test_data[d], tree)
        reel = test_labels[d]
        if predict(test_data[d], tree) == test_labels[d]:
            acc += 1

    print("Gini accuracy with pre-prune: ", acc / len(test_data))
    draw_tree(tree, 'gini_prune_tree')
