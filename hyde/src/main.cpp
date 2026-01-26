#include <model.h>
#include <bstream.h>
#include <ufbx.h>

int main() {
    ufbx_load_opts opts = { 0 };
    ufbx_error error;

    ufbx_scene* scene = ufbx_load_file("D:\\SZS Tools\\J3DConv\\StackedSpheres.fbx", &opts, &error);
    if (scene == nullptr)
    {
        return -1;
    }

    j3d::Model model(scene);
    model.Export();
	
    return 0;
}
