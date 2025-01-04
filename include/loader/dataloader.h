#ifndef DATALOADER_H
#define DATALOADER_H

#include "tensor/xtensor_lib.h"
#include "loader/dataset.h"
#include "list/listheader.h"

using namespace std;

template <typename DType, typename LType>
class DataLoader
{
public:
  class Iterator;
  int current_index;

private:
  Dataset<DType, LType> *ptr_dataset;
  int batch_size;
  bool shuffle;
  bool drop_last;
  xt::xarray<unsigned long> indices;
  XArrayList<Batch<DType, LType>> batch;
  int m_seed;
  int num_samples;
  int num_batches;
  int remain_samples;

public:
  DataLoader(Dataset<DType, LType> *ptr_dataset,
             int batch_size, bool shuffle = true,
             bool drop_last = false, int seed = -1)
      : ptr_dataset(ptr_dataset), batch_size(batch_size), shuffle(shuffle), drop_last(drop_last)
  {
    m_seed = seed;

    indices = xt::arange(ptr_dataset->len());

    num_samples = ptr_dataset->len();
    num_batches = num_samples / batch_size;
    remain_samples = num_samples % batch_size;
  }

  virtual ~DataLoader()
  {
    this->batch.clear();
  }

  class Iterator
  {
  private:
    DataLoader<DType, LType> *data_loader;
    int current_index;

  public:
    Iterator(DataLoader<DType, LType> *loader, int index) : data_loader(loader), current_index(index) {}

    Iterator &operator=(const Iterator &iterator)
    {
      if (this != &iterator)
      {
        data_loader = iterator.data_loader;
        current_index = iterator.current_index;
      }
      return *this;
    }

    Iterator &operator++()
    {
      ++current_index;
      return *this;
    }

    Iterator operator++(int)
    {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator!=(const Iterator &other) const
    {
      return this->current_index != other.current_index;
    }

    Batch<DType, LType> operator*() const
    {

      int current_batch_size = (current_index == data_loader->num_batches - 1 && data_loader->remain_samples > 0 && !data_loader->drop_last)
                                   ? (data_loader->remain_samples + data_loader->batch_size)
                                   : data_loader->batch_size;
      int j = current_index * data_loader->batch_size;
      int n = j + current_batch_size;

      DataLabel<DType, LType> dt = this->data_loader->ptr_dataset->getitem(data_loader->indices[j]);

      xt::xarray<DType> tmp_data = dt.getData();

      xt::xarray<LType> tmp_label = dt.getLabel();

      std::vector<size_t> data_shape(tmp_data.shape().begin(), tmp_data.shape().end());

      std::vector<size_t> label_shape(tmp_label.shape().begin(), tmp_label.shape().end());
      // cout << "4" << endl;
      // cout.flush();

      data_shape.insert(data_shape.begin(), data_loader->batch_size);
      // cout << "5" << endl;
      // cout.flush();

      label_shape.insert(label_shape.begin(), data_loader->batch_size);


      xt::xarray<DType> data = xt::zeros<DType>(data_shape);
      xt::xarray<LType> label = xt::empty<LType>(label_shape);

      for (int k = j; k < n; k++)
      {
        DataLabel<DType, LType> dt = this->data_loader->ptr_dataset->getitem(data_loader->indices[k]);

        xt::view(data, (k - j), xt::all()) = dt.getData();
        xt::view(label, (k - j), xt::all()) = dt.getLabel();
      }

      Batch<DType, LType> phuong(data, label);
      return phuong;

      // return this->data_loader->batch.get(this->current_index);
    }
  };

  Iterator begin()
  {
    if (shuffle && m_seed >= 0)
      xt::random::seed(m_seed);

    if (shuffle)

      xt::random::shuffle(indices);

    return Iterator(this, 0);
  }

  Iterator end()
  {
    if (drop_last)
      return Iterator(this, this->num_batches);
    else
      return Iterator(this, this->batch.size());
    
    
    // return Iterator(this, num_batches + (drop_last ? 0 : (remain_samples > 0 ? 1 : 0)));

  }
};

#endif /* DATALOADER_H */
