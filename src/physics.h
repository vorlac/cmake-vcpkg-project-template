#pragma once
#ifndef PHYSICS_H
  #define PHYSICS_H

  #ifdef __cplusplus
extern "C"
{
  #endif

    void physics_init(void);
    void physics_shutdown(void);

    void physics_update(double delta);

  #ifdef __cplusplus
}
  #endif

#endif
