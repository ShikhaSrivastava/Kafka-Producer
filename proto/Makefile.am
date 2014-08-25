# BEGIN: Variables to change.

# The directory(ies) where protocol buffer files reside.  Each
# directory in this variables is passed to 'protoc' via the '-I'
# option
proto_dirs =						\
	./

# list of Protocol Buffer files comprising shared library.  Provide
# either relative or full path
proto_files =						\
	./event_bus.proto				\
	./EventJournaller.proto				\
	./ae_log.proto					\
	./types.proto					\
	./num_types.proto				\
	./ip.proto					\
	./geo.proto					\
	./ae_types.proto				\
	./ae_inventory.proto				\
	./ae_publisher_user.proto			\
	./adsafe.proto					\
	./beacon.proto					\
	./test-pb-definition.proto	

# List of generated C++ files.  This is a hack to get around the automake bug.
# Only the name of all files in proto_files need to be added here, ie. no
# directories
gen_proto_files =					\
	event_bus.pb.cc					\
	EventJournaller.pb.cc				\
	ae_log.pb.cc					\
	types.pb.cc					\
	num_types.pb.cc					\
	ip.pb.cc					\
	geo.pb.cc					\
	ae_types.pb.cc					\
	ae_inventory.pb.cc				\
	ae_publisher_user.pb.cc				\
	adsafe.pb.cc					\
	beacon.pb.cc					\
	test-pb-definition.pb.cc

# END: Variables to change
# past here, hopefully, there is no need to edit anything
include morpheus-proto.am

# Created on Tue Jan 29 2013 by morpheus Version 2.0.3
