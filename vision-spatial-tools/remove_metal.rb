require 'xcodeproj'

project_path = 'VisionSpatialTools/VisionSpatialTools.xcodeproj'
project = Xcodeproj::Project.open(project_path)

target = project.targets.find { |t| t.name == 'VisionSpatialTools' }

if target.nil?
  puts "Target not found"
  exit 1
end

# Find the file reference
file_path = 'VisionSpatialTools/Shaders/StereoShader.metal'

file_ref = project.main_group.files.find { |f| f.path == file_path }

if file_ref
  # Remove from compile sources
  target.source_build_phase.files_references.delete(file_ref)
  
  # Also remove from build phase files
  target.source_build_phase.files.each do |build_file|
    if build_file.file_ref && build_file.file_ref.path == file_path
      build_file.remove_from_project
    end
  end
  
  # Remove from group
  file_ref.remove_from_project
  puts "Successfully removed StereoShader.metal from Xcode project."
else
  # Maybe it's not in the root group, search recursively
  file_ref = project.files.find { |f| f.path == file_path }
  if file_ref
    target.source_build_phase.files.each do |build_file|
      if build_file.file_ref && build_file.file_ref.path == file_path
        build_file.remove_from_project
      end
    end
    file_ref.remove_from_project
    puts "Successfully removed StereoShader.metal from Xcode project (deep search)."
  else
    puts "File reference not found in project."
  end
end

project.save
