
Pod::Spec.new do |s|
  s.name         = "RNAliOnepass"
  s.version      = "1.0.0"
  s.summary      = "RNAliOnepass"
  s.description  = "RNAliOnepass"
  s.homepage     = "https://github.com/yyyyq/react-native-ali-onepass"
  s.license      = "MIT"
  s.author             = { "lvnini" => "lv@lvnini.cn" }
  s.platform     = :ios, "7.0"
  s.source       = { :git => "https://github.com/lvnini/react-native-ali-one-pass.git", :tag => "master" }
  s.source_files  = "ios/*.{h,m}"
  s.resources =  'ios/libs/ATAuthSDK.framework/ATAuthSDK.bundle'
  s.vendored_frameworks = 'ios/libs/**/*.framework'
  s.requires_arc = true


  s.dependency "React"
  #s.dependency "others"

end

  
