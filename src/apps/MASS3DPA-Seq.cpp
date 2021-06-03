//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-20, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "MASS3DPA.hpp"

#include "RAJA/RAJA.hpp"

#include <iostream>

namespace rajaperf {
namespace apps {

#define RAJA_UNROLL(N)
#define FOREACH_THREAD(i, k, N) for (int i = 0; i < N; i++)

void MASS3DPA::runSeqVariant(VariantID vid) {
  const Index_type run_reps = getRunReps();

  MASS3DPA_DATA_SETUP;

  switch (vid) {

  case Base_Seq: {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      for (int e = 0; e < NE; ++e) {

        MASS3DPA_0_CPU

        FOREACH_THREAD(dy, y, D1D) {
          FOREACH_THREAD(dx, x, D1D){MASS3DPA_1}
          FOREACH_THREAD(dx, x, Q1D) {MASS3DPA_2}
        }

        FOREACH_THREAD(dy, y, D1D) {
          FOREACH_THREAD(qx, x, Q1D) { MASS3DPA_3 }
        }

        FOREACH_THREAD(qy, y, Q1D) {
          FOREACH_THREAD(qx, x, Q1D) { MASS3DPA_4 }
        }

        FOREACH_THREAD(qy, y, Q1D) {
          FOREACH_THREAD(qx, x, Q1D) { MASS3DPA_5 }
        }

        FOREACH_THREAD(d, y, D1D) {
          FOREACH_THREAD(q, x, Q1D) { MASS3DPA_6 }
        }

        FOREACH_THREAD(qy, y, Q1D) {
          FOREACH_THREAD(dx, x, D1D) { MASS3DPA_7 }
        }

        FOREACH_THREAD(dy, y, D1D) {
          FOREACH_THREAD(dx, x, D1D) { MASS3DPA_8 }
        }

        FOREACH_THREAD(dy, y, D1D) {
          FOREACH_THREAD(dx, x, D1D) { MASS3DPA_9 }
        }

      } // element loop
    }
    stopTimer();

    break;
  }

#if defined(RUN_RAJA_SEQ)
  case RAJA_Seq: {

    startTimer();
    for (RepIndex_type irep = 0; irep < run_reps; ++irep) {

      RAJA::expt::launch<launch_policy>(
          RAJA::expt::HOST,
          RAJA::expt::Resources(RAJA::expt::Teams(NE),
                                RAJA::expt::Threads(Q1D, Q1D, 1)),
          [=] RAJA_HOST_DEVICE(RAJA::expt::LaunchContext ctx) {
            RAJA::expt::loop<teams_x>(ctx, RAJA::RangeSegment(0, NE), [&](int e) {

                  MASS3DPA_0_CPU

                  RAJA::expt::loop<threads_y>(ctx, RAJA::RangeSegment(0, D1D), [&](int dy) {
                      RAJA::expt::loop<threads_x>(ctx, RAJA::RangeSegment(0, D1D), [&](int dx) {
                          MASS3DPA_1
                       });

                      RAJA::expt::loop<threads_x>(ctx, RAJA::RangeSegment(0, Q1D), [&](int dx) {
                          MASS3DPA_2
                      });
                   });

                  ctx.teamSync();

                  RAJA::expt::loop<threads_y>(ctx, RAJA::RangeSegment(0, D1D), [&](int dy) {
                      RAJA::expt::loop<threads_x>(ctx, RAJA::RangeSegment(0, Q1D), [&](int qx) {
                          MASS3DPA_3
                      });
                   });

                  ctx.teamSync();

                  RAJA::expt::loop<threads_y>(ctx, RAJA::RangeSegment(0, Q1D), [&](int qy) {
                      RAJA::expt::loop<threads_x>(ctx, RAJA::RangeSegment(0, Q1D), [&](int qx) {
                          MASS3DPA_4
                      });
                  });

                  ctx.teamSync();

                  RAJA::expt::loop<threads_y>(ctx, RAJA::RangeSegment(0, Q1D), [&](int qy) {
                      RAJA::expt::loop<threads_x>(ctx, RAJA::RangeSegment(0, Q1D), [&](int qx) {
                          MASS3DPA_5
                      });
                  });

                  ctx.teamSync();

                  RAJA::expt::loop<threads_y>(ctx, RAJA::RangeSegment(0, D1D), [&](int d) {
                    RAJA::expt::loop<threads_x>(ctx, RAJA::RangeSegment(0, Q1D), [&](int q) {
                        MASS3DPA_6
                     });
                  });

                  ctx.teamSync();

                  RAJA::expt::loop<threads_y>(ctx, RAJA::RangeSegment(0, Q1D), [&](int qy) {
                    RAJA::expt::loop<threads_x>(ctx, RAJA::RangeSegment(0, D1D), [&](int dx) {
                        MASS3DPA_7
                     });
                  });

                  ctx.teamSync();

                  RAJA::expt::loop<threads_y>(ctx, RAJA::RangeSegment(0, D1D), [&](int dy) {
                      RAJA::expt::loop<threads_x>(ctx, RAJA::RangeSegment(0, D1D), [&](int dx) {
                          MASS3DPA_8
                      });
                  });

                  ctx.teamSync();

                  RAJA::expt::loop<threads_y>(ctx, RAJA::RangeSegment(0, D1D), [&](int dy) {
                    RAJA::expt::loop<threads_x>(ctx, RAJA::RangeSegment(0, D1D), [&](int dx) {
                        MASS3DPA_9
                    });
                  });
              });
          });
    }
    stopTimer();

    return;
  }
#endif // RUN_RAJA_SEQ

  default:
    std::cout << "\n MASS3DPA : Unknown Seq variant id = " << vid << std::endl;
  }
}

} // end namespace apps
} // end namespace rajaperf
