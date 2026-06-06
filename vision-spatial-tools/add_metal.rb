require 'xcodeproj'

project_path = '/Users/motonishikoudai/vision-spatial-tools/VisionSpatialTools/VisionSpatialTools.xcodeproj'
file_path = '/Users/motonishikoudai/vision-spatial-tools/VisionSpatialTools/VisionSpatialTools/Shaders/CompositorShaders.metal'

project = Xcodeproj::Project.open(project_path)
target = project.targets.first

# Check if file is already in the project
existing_file = project.files.find { |f| f.real_path.to_s == file_path }

unless existing_file
  # Add the file to the 'Shaders' group or create the group
  main_group = project.main_group.find_subpath('VisionSpatialTools/Shaders', true)
  main_group.set_source_tree('<group>')
  
  file_reference = main_group.new_reference(file_path)
  
  # Add to the compile sources phase
  target.source_build_phase.add_file_reference(file_reference)
  
  project.save
  puts "Added CompositorShaders.metal to project."
else
  puts "File already exists in project."
end
