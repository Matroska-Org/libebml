project('libebml', 'cpp',
  version : '1.4.2',
  default_options : [ 'warning_level=1' ],
)

cpp = meson.get_compiler('cpp')
if host_machine.system() == 'windows' and cpp.has_header('winapifamily.h')
  add_project_arguments('-DHAVE_WINAPIFAMILY_H', language : 'cpp')
endif

if get_option('buildtype') == 'static'
  add_project_arguments('-DEBML_STATIC_DEFINE', language : 'cpp')
endif

ebml_export = configure_file(output: 'ebml_export.h', configuration: {'EBML_DLL_API': ''})
libebml_incdir = include_directories(['.'])

install_headers(
  'ebml/Debug.h',
  'ebml/EbmlBinary.h',
  'ebml/EbmlConfig.h',
  'ebml/EbmlContexts.h',
  'ebml/EbmlCrc32.h',
  'ebml/EbmlDate.h',
  'ebml/EbmlDummy.h',
  'ebml/EbmlElement.h',
  'ebml/EbmlEndian.h',
  'ebml/EbmlFloat.h',
  'ebml/EbmlHead.h',
  'ebml/EbmlId.h',
  'ebml/EbmlMaster.h',
  'ebml/EbmlSInteger.h',
  'ebml/EbmlStream.h',
  'ebml/EbmlString.h',
  'ebml/EbmlSubHead.h',
  'ebml/EbmlTypes.h',
  'ebml/EbmlUInteger.h',
  'ebml/EbmlUnicodeString.h',
  'ebml/EbmlVersion.h',
  'ebml/EbmlVoid.h',
  'ebml/IOCallback.h',
  'ebml/MemIOCallback.h',
  'ebml/MemReadIOCallback.h',
  'ebml/SafeReadIOCallback.h',
  'ebml/StdIOCallback.h',
  ebml_export,
  subdir : 'ebml',
)

install_headers(
  'ebml/c/libebml_t.h',
  subdir: 'ebml/c/',
)

cpp_files = [
  'src/Debug.cpp',
  'src/EbmlBinary.cpp',
  'src/EbmlContexts.cpp',
  'src/EbmlCrc32.cpp',
  'src/EbmlDate.cpp',
  'src/EbmlDummy.cpp',
  'src/EbmlElement.cpp',
  'src/EbmlFloat.cpp',
  'src/EbmlHead.cpp',
  'src/EbmlMaster.cpp',
  'src/EbmlSInteger.cpp',
  'src/EbmlStream.cpp',
  'src/EbmlString.cpp',
  'src/EbmlSubHead.cpp',
  'src/EbmlUInteger.cpp',
  'src/EbmlUnicodeString.cpp',
  'src/EbmlVersion.cpp',
  'src/EbmlVoid.cpp',
  'src/IOCallback.cpp',
  'src/MemIOCallback.cpp',
  'src/MemReadIOCallback.cpp',
  'src/SafeReadIOCallback.cpp',
  'src/StdIOCallback.cpp',
]

if get_option('win32_io') and host_machine.system() == 'windows'
  cpp_files += 'src/platform/win32/WinIOCallback.cpp'
endif

libebml = library('ebml',
  cpp_files,
  version : '5.0.0',
  include_directories : libebml_incdir,
  install : true,
)

pkg = import('pkgconfig')
pkg.generate(libebml, name: 'libebml')

libebml_dep = declare_dependency(
  include_directories : libebml_incdir,
  link_with : libebml,
)
