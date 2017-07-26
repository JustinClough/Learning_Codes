# Use this to create models and corresponding meshes

box_dir=/lore/clougj/scorec/build/test/

nx=1
ny=1
nz=0
wx=1 
wy=1 
wz=0 
quad_tri=0
model_name=2D_quad.dmg
mesh_name=2D_quad.smb


$box_dir/box \
  $nx \
  $ny \
  $nz \
  $wx \
  $wy \
  $wz \
  $quad_tri   \
  $mesh_name \
  $model_name 
