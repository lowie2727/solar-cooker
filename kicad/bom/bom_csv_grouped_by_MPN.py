# Import the required modules
import kicad_netlist_reader
import kicad_utils
import csv
import sys
import os

# A helper function to filter/convert a string read in netlist
# currently: do nothing
def fromNetlistText(aText):
    return aText

def myEqu(self, other):
    """
    myEqu is a more advanced equivalence function for components which is
    used by component grouping. Normal operation is to group components based
    on their value and footprint.

    In this example of a custom equivalency operator we compare the
    value, the part name and the footprint.
    """
    result = True
    if self.getValue() != other.getValue():
        result = False
    elif self.getPartName() != other.getPartName():
        result = False
    elif self.getFootprint() != other.getFootprint():
        result = False
    elif self.getDNP() != other.getDNP():
        result = False

    return result

# Override the component equivalence operator - it is important to do this
# before loading the netlist, otherwise all components will have the original
# equivalency operator.
kicad_netlist_reader.comp.__eq__ = myEqu

if len(sys.argv) != 3:
    print("Usage ", __file__, "<generic_netlist.xml> <output.csv>", file=sys.stderr)
    sys.exit(1)

# Generate an instance of a generic netlist, and load the netlist tree from
# the command line option. If the file doesn't exist, execution will stop
net = kicad_netlist_reader.netlist(sys.argv[1])

# Open a file to write to, if the file cannot be opened output to stdout
output_dir = os.path.join(os.path.dirname(sys.argv[2]), "output")
os.makedirs(output_dir, exist_ok=True)  # Create the output directory if it doesn't exist
output_filename = os.path.join(output_dir, "solar_cooker_v1.5.csv")

# Open a file to write to, if the file cannot be opened, output to stdout instead
try:
    f = kicad_utils.open_file_writeUTF8(output_filename, 'w')
except IOError:
    e = "Can't open output file for writing: " + output_filename
    print(__file__, ":", e, sys.stderr)
    f = sys.stdout

# subset the components to those wanted in the BOM, controlled
# by <configure> block in kicad_netlist_reader.py
components = net.getInterestingComponents(excludeBOM=True)

# Create a new csv writer object to use as the output formatter
out = csv.writer(f, lineterminator='\n', delimiter=',', quotechar='\"', quoting=csv.QUOTE_ALL)

# Override csv.writer's writerow() to support encoding conversion (initial encoding is utf8)
def writerow(acsvwriter, columns):
    utf8row = []
    for col in columns:
        utf8row.append(fromNetlistText(str(col)))
    acsvwriter.writerow(utf8row)

# Output a set of rows as a header providing general information
writerow(out, ['Source:', net.getSource()])
writerow(out, ['Date:', net.getDate()])
writerow(out, ['Tool:', net.getTool()])
writerow(out, ['Generator:', sys.argv[0]])
writerow(out, ['Component Count:', len(components)])
writerow(out, [])

writerow(out, ['Collated Components:'])
writerow(out, [])
writerow(out, ['Reference', 'Quantity', 'Manufacturer', 'MPN', 'Supplier', 'SPN', 'Component package type', 'Description'])

# Group the components based on MPN field
groups = {}
for component in components:
    spn = component.getField("SPN")
    if spn in groups:
        groups[spn].append(component)  # Add component to existing group
    else:
        groups[spn] = [component]  # Initialize a new group

# Output component information organized by group, aka collated
for spn, components in groups.items():
    row = [
        ", ".join(component.getRef() for component in components),  # Combine references of components in the group
        len(components),  # Quantity of components in the group
        components[0].getField("Manufacturer"),  # Manufacturer (assuming the first component in the group)
        components[0].getField("MPN"),  # MPN
        components[0].getField("Supplier"),  # Supplier (assuming the first component in the group)
        spn,  # SPN (assuming the first component in the group)
        components[0].getField("Component package type"),  # Component package type (assuming the first component in the group)
        components[0].getField("Description")  # Description (assuming the first component in the group)
    ]
    writerow(out, row)

# close the output file
f.close()
