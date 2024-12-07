#include "stdafx.h"
#include "camera.h"

Camera Camera::Global[16];
Camera* Camera::pCurrent = &Camera::Global[0];