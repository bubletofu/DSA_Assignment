#ifndef DATASET_H
#define DATASET_H
#include "xtensor_lib.h"
#include "list/XArrayList.h"
#include "tensor/xtensor_lib.h"

using namespace std;

template <typename DType, typename LType>
class DataLabel
{
private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;

public:
  DataLabel(xt::xarray<DType> data, xt::xarray<LType> label)
      : data(data), label(label) {}
  xt::xarray<DType> getData() const { return data; }
  xt::xarray<LType> getLabel() const { return label; }
};

template <typename DType, typename LType>
class Batch
{
private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;

public:
  Batch() : data(), label() {}
  Batch(xt::xarray<DType> data, xt::xarray<LType> label)
      : data(data), label(label) {}

  bool operator==(const Batch &other) const
  {
    return (this->data == other.data) && (this->label == other.label);
  }

  friend std::ostream &operator<<(std::ostream &os, const Batch &batch)
  {
    os << "Data: " << batch.data << "\n";
    os << "Label: " << batch.label << "\n";
    return os;
  }

  virtual ~Batch() {}
  xt::xarray<DType> &getData() { return data; }
  xt::xarray<LType> &getLabel() { return label; }
};

template <typename DType, typename LType>
class Dataset
{
public:
  Dataset() {};
  virtual ~Dataset() {};

  virtual int len() = 0;
  virtual DataLabel<DType, LType> getitem(int index) = 0;
  virtual xt::svector<unsigned long> get_data_shape() = 0;  // Change type here
  virtual xt::svector<unsigned long> get_label_shape() = 0; // Change type here
};

template <typename DType, typename LType>
class TensorDataset : public Dataset<DType, LType>
{
private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;
  xt::svector<unsigned long> data_shape, label_shape; // Change type here

public:
  TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label)
      : data(data), label(label)
  {
    this->data = data;
    this->label = label;
    data_shape = xt::svector<unsigned long>(data.shape().begin(), data.shape().end());
    if (label.dimension() != 0)
      label_shape = xt::svector<unsigned long>(label.shape().begin(), label.shape().end());

    // cout << xt::adapt(data_shape) << endl;
    // cout << label_shape.size() << endl;
    // cout.flush();
  }

  int len() override
  {
    // return data_shape[0];
    return this->data.shape()[0];
  }
  DataLabel<DType, LType> getitem(int index) override
  {
    if (index < 0 || index >= this->len())
      throw std::out_of_range("Index is out of range!");

    xt::xarray<DType> data_item = xt::view(data, index);
    xt::xarray<LType> label_item = this->label;


    if (label_shape.size() > 0)
    {
      label_item = xt::view(label, index);
    }
    // else
    // {
    //   label_item = xt::empty<LType>({});
    // }
    DataLabel<DType, LType> abc(data_item, label_item);

    return abc;
  }

  xt::svector<unsigned long> get_data_shape() override { return data_shape; }

  xt::svector<unsigned long> get_label_shape() override
  {
    if (label_shape.size() > 0)
    {
      return label_shape;
    }
    else
    {
      return xt::svector<size_t>{};
    }
  }
};

#endif /* DATASET_H */
