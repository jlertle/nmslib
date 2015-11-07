/**
 * Non-metric Space Library
 *
 * Authors: Bilegsaikhan Naidan (https://github.com/bileg), Leonid Boytsov (http://boytsov.info).
 * With contributions from Lawrence Cayton (http://lcayton.com/) and others.
 *
 * For the complete list of contributors and further details see:
 * https://github.com/searchivarius/NonMetricSpaceLib 
 * 
 * Copyright (c) 2014
 *
 * This code is released under the
 * Apache License Version 2.0 http://www.apache.org/licenses/.
 *
 */

#include "space.h"
#include "rangequery.h"
#include "knnquery.h"
#include "method/seqsearch.h"

namespace similarity {

template <typename dist_t>
SeqSearch<dist_t>::SeqSearch(const ObjectVector& origData) : 
                      origData_(origData), cacheOptimizedBucket_(NULL), pData_(NULL) {
}

template <typename dist_t>
void SeqSearch<dist_t>::CreateIndex(const AnyParams& IndexParams) {
  AnyParamManager pmgr(IndexParams);

  bool bCopyMem;
  pmgr.GetParamOptional("copyMem", bCopyMem, false);

  LOG(LIB_INFO) << "copyMem = " << bCopyMem;

  SetQueryTimeParams(getEmptyParams());

  if (bCopyMem) {
    CreateCacheOptimizedBucket(origData_, cacheOptimizedBucket_, pData_);
  }
}

template <typename dist_t>
SeqSearch<dist_t>::~SeqSearch() {
  if (cacheOptimizedBucket_ != NULL) {
    ClearBucket(cacheOptimizedBucket_, pData_);
  }
}

template <typename dist_t>
void SeqSearch<dist_t>::Search(RangeQuery<dist_t>* query, IdType) const {
  const ObjectVector& data = pData_ != NULL ? *pData_ : origData_;
  for (size_t i = 0; i < data.size(); ++i) {
    query->CheckAndAddToResult(data[i]);
  }
}

template <typename dist_t>
void SeqSearch<dist_t>::Search(KNNQuery<dist_t>* query, IdType) const {
  const ObjectVector& data = pData_ != NULL ? *pData_ : origData_;
  for (size_t i = 0; i < data.size(); ++i) {
    query->CheckAndAddToResult(data[i]);
  }
}

template class SeqSearch<float>;
template class SeqSearch<double>;
template class SeqSearch<int>;

}
