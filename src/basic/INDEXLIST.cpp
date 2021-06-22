//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-21, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "INDEXLIST.hpp"

#include "RAJA/RAJA.hpp"

#include "common/DataUtils.hpp"

namespace rajaperf
{
namespace basic
{


INDEXLIST::INDEXLIST(const RunParams& params)
  : KernelBase(rajaperf::Basic_INDEXLIST, params)
{
  setDefaultSize(1000000);
  setDefaultReps(100);

  setVariantDefined( Base_Seq );
  setVariantDefined( Lambda_Seq );
  setVariantDefined( RAJA_Seq );

#if defined(_OPENMP) && _OPENMP >= 201811
  setVariantDefined( Base_OpenMP );
  setVariantDefined( Lambda_OpenMP );
#endif
}

INDEXLIST::~INDEXLIST()
{
}

void INDEXLIST::setUp(VariantID vid)
{
  allocAndInitDataRandSign(m_x, getRunSize(), vid);
  allocAndInitData(m_list, getRunSize(), vid);
  m_len = -1;
}

void INDEXLIST::updateChecksum(VariantID vid)
{
  checksum[vid] += calcChecksum(m_list, getRunSize());
  checksum[vid] += Checksum_type(m_len);
}

void INDEXLIST::tearDown(VariantID vid)
{
  (void) vid;
  deallocData(m_x);
  deallocData(m_list);
}

} // end namespace basic
} // end namespace rajaperf
