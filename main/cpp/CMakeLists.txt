#############################################################################
#  This file is part of the Stride software.
#  It is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or any
#  later version.
#  The software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  You should have received a copy of the GNU General Public License,
#  along with the software. If not, see <http://www.gnu.org/licenses/>.
#  see http://www.gnu.org/licenses/.
#
#  Copyright 2017, 2018 Willem L, Kuylen E & Broeckhove J
#############################################################################

#============================================================================
# Sources. Includes headers to enable code insight by CLion.
#============================================================================

set(LIB_SRC
#---
	behaviour/BeliefSeeder.cpp
	behaviour/behaviour_policies/NoBehaviour.h
	behaviour/behaviour_policies/Vaccination.h
	behaviour/belief_policies/Belief.h
	behaviour/belief_policies/HBM.h
	behaviour/belief_policies/Imitation.h
	behaviour/belief_policies/NoBelief.h
	behaviour/belief_policies/Threshold.h
	behaviour/information_policies/LocalDiscussion.h
	behaviour/information_policies/NoGlobalInformation.h
	behaviour/information_policies/NoLocalInformation.h
#---
    calendar/Calendar.cpp
    calendar/DaysOffAll.h
    calendar/DaysOffInterface.h
    calendar/DaysOffNone.h
    calendar/DaysOffSchool.h
    calendar/DaysOffStandard.h
#---
	contact/AgeContactProfile.cpp
	contact/AgeContactProfiles.h
	contact/ContactHandler.h
	contact/ContactLogMode.cpp
	contact/Infector.cpp
	contact/InfectorExec.h
	contact/InfectorMap.h
    contact/TransmissionProfile.cpp
#---
    disease/DiseaseSeeder.cpp
	disease/Health.cpp
    disease/HealthSeeder.cpp
	disease/Immunizer.cpp
#---
	output/AdoptedFile.cpp
    output/InfectedFile.cpp
    output/PersonsFile.cpp
	output/SummaryFile.cpp
#---
	pool/ContactPool.cpp
	pool/ContactPoolSys.h
	pool/ContactPoolType.cpp
    pool/IdSubscriptArray.h
#---
    pop/Age.h
    pop/Person.cpp
    pop/Population.cpp
	pop/PopBuilder.cpp
    pop/SurveySeeder.cpp
#---
    gen/popgen/PopGenerator.cpp
    gen/popgen/assigners/HouseholdAssigner.cpp
    gen/popgen/assigners/SchoolAssigner.cpp
    gen/popgen/assigners/UniversityAssigner.cpp
    gen/popgen/assigners/WorkplaceAssigner.cpp
    gen/popgen/assigners/CommunityAssigner.cpp
	gen/popgen/assigners/TravelAssigner.cpp
    gen/GenConfiguration.cpp
    gen/geogen/GeoGenerator.cpp
    gen/geogen/builders/GeoGridBuilder.cpp
    gen/geogen/builders/PopulationBuilder.cpp
    gen/geogen/builders/SchoolsBuilder.cpp
    gen/geogen/builders/UniversitiesBuilder.cpp
    gen/geogen/builders/WorkplacesBuilder.cpp
    gen/geogen/builders/CommunitiesBuilder.cpp
	gen/files/GenFile.cpp
	gen/files/GenDirectory.cpp
    gen/files/PopulationFile.cpp
#---
    sim/CliController.cpp
	sim/GuiController.cpp
	sim/SimRunner.cpp
    sim/Sim.cpp
    sim/SimBuilder.cpp
    sim/event/Id.cpp
	sim/event/Subject.cpp
#---
    util/Any.h
    util/ConfigInfo.cpp
    util/CSV.cpp
	util/CSVRow.cpp
    util/FileSys.cpp
	util/LogUtils.cpp
    util/RNEngineType.cpp
    util/RNManager.cpp
    util/RunConfigManager.cpp
    util/SegmentedVector.h
    util/Stopwatch.h
    util/StringUtils.h
    util/Subject.h
    util/SVIterator.h
    util/TimeStamp.h
    util/TimeToString.h
	util/StatisticsRetriever.h
	util/StatisticsRetriever.cpp
    util/InstallDirs.cpp
    util/GeometryGeoCoord.cpp
    util/Regions.cpp
#---
    ui/editconfigform.cpp
    ui/geogendata.cpp
    ui/geogenvisualization.cpp
    ui/geogridlocation.cpp
	ui/mainwindow.cpp
    ui/mapviewerwindow.cpp
    ui/peryiteuidata.cpp
    ui/popgendata.cpp
    ui/popgenvisualization.cpp
	ui/stridescattergraph.cpp
    ui/stridewindow.cpp
    ui/util.cpp
    ui/visualizationcircle.cpp
    ui/visualizationgraphbar.cpp
#---
	viewers/AdoptedViewer.cpp
	viewers/InfectedViewer.cpp
    viewers/CliViewer.cpp
	viewers/MapViewer.cpp
    viewers/PersonsViewer.cpp
	viewers/SummaryViewer.cpp
        )

#============================================================================
# Automatically generate ui header files for Qt
#============================================================================
SET(CMAKE_AUTOUIC ON)

#============================================================================
# Build the Stride library.
#============================================================================
add_library(libstride ${LIB_SRC})
set_source_files_properties(BeliefSeeder.cpp PROPERTIES COMPILE_FLAGS "-Wno-unused-variable")
target_link_libraries(libstride ${LIBS})
target_compile_options(libstride PUBLIC "-fPIC")
target_compile_definitions(libstride PUBLIC "-DSTRIDE_GIT_HASH=${STRIDE_WC_REVISION_HASH}")
set_target_properties(libstride PROPERTIES PREFIX "")

#============================================================================
# Build and install the stride executable.
#============================================================================
add_executable(stride sim/main.cpp)
target_link_libraries(stride ${LIBS} libstride Qt5::Core Qt5::Widgets Qt5::Charts)
install(TARGETS stride  DESTINATION   ${BIN_INSTALL_LOCATION})

#============================================================================
# Clean up.
#============================================================================
unset(MAIN_SRC)

#############################################################################
