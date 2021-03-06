//
// Created by hua on 19-3-4.
//

#ifndef LOADPARAM_NET_HPP
#define LOADPARAM_NET_HPP

#include <caffe.pb.h>
#include <vector>
#include <map>
#include <memory>
#include <string>
//#include "transformer_param.hpp"
#include "upgrade_proto.hpp"
#include "blob.hpp"
#include "layer.hpp"

using std::vector;
using std::map;
using std::string;

namespace caffe{

class Net{
public:
    /// @brief 显示构造函数：网络模型文件, 训练参数文件
    explicit Net(const string& model_file, const string& trained_file);

    /// @brief 使用NetParameter初始化网络
    void Init(const NetParameter& param);

    /// @brief 前向计算，输入：处理数据、计算开始的位置+结束的位置，返回：结束位置对应的运算结果
    const Blob* Forward(const Blob<Dtype>& input_data, const string& begin, const string& end);

protected:
    ;
private:
    // Follow a sequence stored every layer's parameter.
    // Traversal layer's parameter layer by layer from layer id.
    NetParameter net_parameter_;
    // a map contained objects of kinds of layers
    map<string, shared_ptr<Layer> > layers_;
    // bottom
    map<string, vector<shared_ptr<Blob>> > bottom_vecs_;
    // top
    map<string, vector<shared_ptr<Blob>> > top_vecs_;
};

}

#endif //LOADPARAM_NET_HPP
