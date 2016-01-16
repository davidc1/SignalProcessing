#
# Example PyROOT script to run analysis module, ana_base.
# The usage is same for inherited analysis class instance.
#

# Load libraries
import os, ROOT, sys
from larlite import larlite as fmwk
from ROOT import signalana
# Now import ana_processor & your class. For this example, ana_base.
#from ROOT import larlite as fmwk

# Create ana_processor instance
my_proc=fmwk.ana_processor()

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

my_proc.set_output_file("deco.root")
my_proc.set_ana_output_file("deco_ana.root")

deconvolution_tool_beam = signalana.DecoInterface(32,2048,64e6)
deconvolution_tool_beam.setPadding(5)
deconvolution_tool_beam.setName("beam")
deconvolution_tool_beam.setDebug(0)
deconvolution_tool_beam.setBaseline(2048)
deconvolution_tool_beam.setPMTNum(32)
deconvolution_tool_cosmic = signalana.DecoInterface(32,256,64e6)
deconvolution_tool_cosmic.setPadding(5)
deconvolution_tool_cosmic.setName("cosmic")
deconvolution_tool_cosmic.setDebug(0)
deconvolution_tool_cosmic.setBaseline(2048)
deconvolution_tool_cosmic.setPMTNum(32)

deco = fmwk.Deco()
#original_producer = 'pmtreadout'
original_producer = 'saturation'
deco.setProducer(original_producer);
deco.setSPEKernelFile('specalib_run3692.root')
deco.setFilterFileBeam('filters_mucs.root')
#deco.setFilterFileBeam('filters.root')
deco.setFilterFileCosmic('filters_cosmic_discriminator.root')
deco.setNumTicksBeam(2048)
deco.setNumTicksCosmic(256)
deco.setDecoToolBeam(deconvolution_tool_beam)
deco.setDecoToolCosmic(deconvolution_tool_cosmic)
my_proc.add_process(deco)

my_proc.set_data_to_write(fmwk.data.kOpDetWaveform,"deconvolution")
my_proc.set_data_to_write(fmwk.data.kOpDetWaveform,original_producer)

my_proc.run(0,1000)
sys.exit(0);
