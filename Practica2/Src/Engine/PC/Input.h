#pragma once

// QUEREMOS QUE DESDE MAIN SOLO SE HAGA INCLUDE DE "Renderer.h"
// DEPENDIENDO DE SI ES x64 || ORBIS LLAMARA AL DE UN PROYECTO U OTRO
// HACEMOS QUE ESTE LLAME AL QUE TIENE EL CODIGO MEDIANTE UN using
#include "InputPC.h"

using Input = InputPC;