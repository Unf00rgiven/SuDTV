/**
 *
 *  http://www.digitalekabeltelevisie.nl/dvb_inspector
 *
 *  This code is Copyright 2009-2015 by Eric Berendsen (e_berendsen@digitalekabeltelevisie.nl)
 *
 *  This file is part of DVB Inspector.
 *
 *  DVB Inspector is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  DVB Inspector is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with DVB Inspector.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  The author requests that he be notified of any application, applet, or
 *  other binary that makes use of this code, but that's more out of curiosity
 *  than anything and is not required.
 *
 */

package nl.digitalekabeltelevisie.data.mpeg.descriptors;

import static nl.digitalekabeltelevisie.util.Utils.*;

import javax.swing.tree.DefaultMutableTreeNode;

import nl.digitalekabeltelevisie.controller.KVP;
import nl.digitalekabeltelevisie.data.mpeg.psi.TableSection;

public abstract class ExtensionDescriptor extends Descriptor {

	protected final int descriptor_tag_extension;
	protected final byte[] selector_byte;

	public ExtensionDescriptor(final byte[] b, final int offset, final TableSection parent) {
		super(b, offset,parent);
		descriptor_tag_extension = getInt(b, privateDataOffset++, 1, MASK_8BITS);
		selector_byte=getBytes(b, privateDataOffset, descriptorLength-1);
	}

	@Override
	public DefaultMutableTreeNode getJTreeNode(final int modus){

		final DefaultMutableTreeNode t = super.getJTreeNode(modus);
		t.add(new DefaultMutableTreeNode(new KVP("descriptor_tag_extension",descriptor_tag_extension,getDescriptorTagString())));
		t.add(new DefaultMutableTreeNode(new KVP("selector_byte",selector_byte,null)));

		return t;
	}

	public String getDescriptorname() {
		return getDescriptorname(descriptorTag, parentTableSection)+" ("+getDescriptorTagString()+")";
	}

	public abstract String getDescriptorTagString();

}
