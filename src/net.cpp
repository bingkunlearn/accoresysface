//
// Created by hua on 19-3-7.
//
#include "net.hpp"
#include "caffe.hpp"
using namespace caffe;

//Net::Net(const string& model_file, const string& trained_file) {
//    NetParameter param;
//    ReadNetParamsFromTextFile(model_file, param);
//    Init(param);  // 初始化网络模型
//}

void Net::Init(const NetParameter& in_param){


    map<string, int> blob_name_to_idx;
    const int layers_size = in_param.layer_size();

    bottom_vecs_.resize(layers_size);
    top_vecs_.resize(layers_size);

    // 循环遍历每一层，进行初始化
    for(int layer_id=0; layer_id<in_param.layer_size(); ++layer_id){
        const LayerParameter& layer_param = in_param.layer(layer_id);
        shared_ptr<Layer> layer_pointer(NULL);
        layer_name_id_[layer_param.name()] = layer_id;
        set<string> available_blobs;

        for(int bottom_id=0; bottom_id<layer_param.bottom_size(); ++bottom_id){
            const string& blob_name = layer_param.bottom(bottom_id);
            if(available_blobs.find(blob_name) != available_blobs.end()){
                const int blob_id = blob_name_to_idx[blob_name];
                bottom_vecs_[layer_id].push_back(blobs_[blob_id].get());
            }
            else{
                std::cout<< "Unknown bottom blob '" << blob_name << "' (layer '"
                         << layer_param.name() << "', bottom index " << bottom_id << ")"
                         <<std::endl;
                exit(0);  // debug
            }

        }

        for(int top_id=0; top_id<layer_param.top_size(); ++top_id){
            shared_ptr<Blob> blob_pointer(new Blob());
            top_vecs_[layer_id].push_back(blob_pointer.get());
            //top_id_vecs_[layer_id].push_back(top_id);
            const string& blob_name = layer_param.top(top_id);
            blob_names_.push_back(blob_name);
            const int blob_id = blobs_.size();
            blobs_.push_back(blob_pointer);
            top_id_vecs_[layer_id].push_back(blob_id);
            blob_name_to_idx[blob_name] = blob_id;
            available_blobs.insert(blob_name);
        }

        //std::cout<<layer_param.type()<<std::endl;
        if("Input" == layer_param.type())
        {
            ;//layer.reset(new InputLayer);
        }
        else if("Convolution" == layer_param.type())
        {
            layer_pointer.reset(new ConvLayer);
        }
        else if("BatchNorm" == layer_param.type()){
            layer_pointer.reset(new BNLayer);
        }
        else if ("Pooling" == layer_param.type())
        {
            layer_pointer.reset(new PoolLayer);
        }
        else if("ReLU" == layer_param.type())
        {
            layer_pointer.reset(new ReluLayer);
        }
        else if("InnerProduct" == layer_param.type()){
            layer_pointer.reset(new FCLayer);
        }
        else if ("Softmax" == layer_param.type())
        {
            ;
            //layer.reset(new SoftmaxLayer);
        }
        //SetUp中传一个输入尺寸，vector<int>: n, c, h, w?
        if(layer_pointer){
            layer_pointer->SetUp(layer_param, bottom_vecs_[layer_id], top_vecs_[layer_id]);  // 打印消息
        }

        layers_.push_back(layer_pointer);
    }
}


