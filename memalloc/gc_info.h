#pragma once

struct GcInfo {
    bool marked = false;
    bool to_be_checked = false;
    bool root = false;
};
