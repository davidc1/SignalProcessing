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

deconvolution_tool_beam = signalana.DecoInterface()
deconvolution_tool_beam.setDebug(3)
deconvolution_tool_cosmic = signalana.DecoInterface()
deconvolution_tool_cosmic.setDebug(3)

deco = fmwk.Deco()
deco.setProducer("saturation");
deco.setSPEKernelFile('specalib_run3692.root')
deco.setFilterFileBeam('filters.root')
deco.setFilterFileCosmic('filters_cosmic_discriminator.root')
deco.setNumTicksBeam(2048)
deco.setNumTicksCosmic(256)
deco.setDecoToolBeam(deconvolution_tool_beam)
deco.setDecoToolCosmic(deconvolution_tool_cosmic)
my_proc.add_process(deco)

my_proc.set_data_to_write(fmwk.data.kOpDetWaveform,"deconvolution")
my_proc.set_data_to_write(fmwk.data.kOpDetWaveform,"saturation")

my_proc.run(0,1000)
sys.exit(0);
