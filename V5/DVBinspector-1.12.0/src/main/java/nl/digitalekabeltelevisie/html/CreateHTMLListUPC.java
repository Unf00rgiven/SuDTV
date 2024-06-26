package nl.digitalekabeltelevisie.html;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.Map;
import java.util.TreeSet;

import nl.digitalekabeltelevisie.data.mpeg.TransportStream;
import nl.digitalekabeltelevisie.data.mpeg.descriptors.CableDeliverySystemDescriptor;
import nl.digitalekabeltelevisie.data.mpeg.descriptors.Descriptor;
import nl.digitalekabeltelevisie.data.mpeg.descriptors.privatedescriptors.upc.UPCLogicalChannelDescriptor;
import nl.digitalekabeltelevisie.data.mpeg.psi.NIT;
import nl.digitalekabeltelevisie.data.mpeg.psi.NITsection;
import nl.digitalekabeltelevisie.data.mpeg.psi.SDT;
import nl.digitalekabeltelevisie.data.mpeg.psi.SDTsection;
import nl.digitalekabeltelevisie.data.mpeg.psi.SDTsection.Service;
import nl.digitalekabeltelevisie.util.Utils;

public class CreateHTMLListUPC implements Runnable{

	private TransportStream transportStream;


	public CreateHTMLListUPC() {
		super();
	}

	/**
	 * @param args
	 */
	public static void main(final String[] args) {
		final CreateHTMLListUPC inspector = new CreateHTMLListUPC();
		inspector.run();
	}

	public void run() {
		try {

			transportStream = new TransportStream("D:\\ts\\upc vorden 20141226\\UPC 164000 12-26 11-24-24.ts");
			transportStream.parseStream();
		} catch (final Exception e) {

			e.printStackTrace();
		}

		System.out.println(transportStream);
		writeHTML(transportStream);
		System.out.println("Klaar!!");


	}

	private void writeHTML(final TransportStream tStream) {

		try {
			final FileWriter fstream = new FileWriter("d:\\eric\\upc20141226.html");
			final BufferedWriter out = new BufferedWriter(fstream);

			out.write("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n<html>\n<head></head>\n");
			out.write("<body>\n");

			out.write("	<table class=\"content\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\">\n");

			final SDT sdt = tStream.getPsi().getSdt();

			final NIT nit = tStream.getPsi().getNit();
			final Map<Integer, NITsection []> networks = nit.getNetworks();

			final TreeSet<Integer> networksSetFreqs = new TreeSet<Integer>(networks.keySet());
			final TreeSet<Integer> networksSet = new TreeSet<Integer>();

			networksSet.add(43136);
			networksSet.add(43138);
			networksSet.add(43140);
			networksSet.add(43142);
			networksSet.add(43144);
			networksSet.add(43146);


			networksSet.add(50142);
			networksSet.add(50146);

			out.write("<thead><tr><th rowspan=\"2\">Zender</th><th rowspan=\"2\">Type (TV/Radio)<br> - (RAW)</th><th rowspan=\"2\">Service<br>ID</th><th rowspan=\"2\">kanaalnummer mediabox</th><th colspan=\"9\">Kanaalnummer CI+ (NIT)</th></tr>\n");

			out.write("<tr>\n");
			for(final Integer nid: networksSet){
				out.write("<th>"+nid +"</th>");

			}


			out.write("</tr></thead>\n");


			out.write("<tbody>\n");

			final Map<Integer, SDTsection[]> streams = sdt.getTransportStreams();
			final TreeSet<Integer> s = new TreeSet<Integer>(streams.keySet());

			for(final Integer transportStreamID: s){
				out.write("<tr><th colspan=\"13\">"+transportStreamID+"</th></tr>\n");
				final SDTsection [] sections = streams.get(transportStreamID);
				final ArrayList<Service> serviceList = new ArrayList<Service>();

				for (final SDTsection section: sections) {
					if(section!= null){
						serviceList.addAll(section.getServiceList());
					}
				}

				Collections.sort(serviceList, new Comparator<Service>(){
					public int compare(final Service s1, final Service s2){
						return(s1.getServiceID()-s2.getServiceID());
					}
				});
				for (final Service element: serviceList) {
					final int sid=element.getServiceID();



					//String safeName= sdt.getServiceName(sid).replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
					final String safeName= Utils.escapeHTML(sdt.getServiceName(sid));
					out.write("<tr><td style=\"text-align: left;\">\n"+safeName+"</td>\n");
					out.write("<td style=\"text-align: left;\">"+Descriptor.getServiceTypeStringShort(sdt.getServiceType(sid))+" ("+sdt.getServiceType(sid)+") </td>\n");
					out.write("<td style=\"text-align: left;\">"+sid+" </td>\n");
					//int lcn = tStream.getPsi().getNit().getLCN(nid, transportStreamID, sid);
					String upclcnString = "-";
					final ArrayList<Descriptor>descList= new ArrayList<Descriptor>();
					descList.addAll(element.getDescriptorList());
					for(final Descriptor d:descList){
						if(d instanceof UPCLogicalChannelDescriptor) {
							upclcnString=((UPCLogicalChannelDescriptor)d).getLogicalChannelNumbers();
						}
					}


					out.write("<td>"+upclcnString +"</td>");

					for(final Integer nid: networksSet){

						final int lcn = tStream.getPsi().getNit().getLCN(nid, transportStreamID, sid);
						final String lcnString = (lcn>0)?Integer.toString(lcn):"-";
						out.write("<td>"+lcnString +"</td>");

					}

					out.write("</tr>\n");
				}


			}

			out.write("	\n");
			out.write("<br><br>	\n");
			out.write("	\n");
			out.write("	</tbody></table>\n");
			out.write("	<p><br></p><p>Onderstaande lijst bevat alle frequenties zoals ze gebruikt worden per regionaal netwerk.</p>\n");
			out.write("	<p><br></p>\n");
			out.write("	<table class=\"content\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\"><thead><tr><th>Transportstream-ID</th><th>Frequentie</th><th>Modulatie</th></tr></thead><tbody>\n");


			final Iterator<Integer> j = networksSetFreqs.iterator();
			while(j.hasNext()){
				final int networkNo=j.next();

				out.write("<tr><th colspan=\"3\">"+networkNo+" : "+nit.getNetworkName(networkNo)+"</th></tr>\n");
				final ArrayList<nl.digitalekabeltelevisie.data.mpeg.psi.NITsection.TransportStream> tstreams = new ArrayList<nl.digitalekabeltelevisie.data.mpeg.psi.NITsection.TransportStream>();
				final NITsection [] nitsections = networks.get(networkNo);

				for(final NITsection section: nitsections){
					if(section!= null){
						tstreams.addAll(section.getTransportStreamList());
					}
				}

				Collections.sort(tstreams, new Comparator<nl.digitalekabeltelevisie.data.mpeg.psi.NITsection.TransportStream>(){
					public int compare(final nl.digitalekabeltelevisie.data.mpeg.psi.NITsection.TransportStream s1, final nl.digitalekabeltelevisie.data.mpeg.psi.NITsection.TransportStream s2){
						return(s1.getTransportStreamID()-s2.getTransportStreamID());
					}
				});

				for (final nl.digitalekabeltelevisie.data.mpeg.psi.NITsection.TransportStream stream: tstreams) {
					final int sid=stream.getTransportStreamID();
					final Iterator<Descriptor> descs=stream.getDescriptorList().iterator();
					String freq="";
					int mod=-1;
					String modulation = "";

					while(descs.hasNext()){
						final Descriptor d=descs.next();
						if(d instanceof CableDeliverySystemDescriptor) {
							freq = Descriptor.formatCableFrequency(((CableDeliverySystemDescriptor)d).getFrequency());
							mod = ((CableDeliverySystemDescriptor)d).getModulation();
							modulation = CableDeliverySystemDescriptor.getModulationString(mod);

						}
					}

					out.write("<tr><td>"+sid+"</td>\n");
					out.write("<td>"+freq+" </td><td>"+modulation+ "</td></tr>\n");

				}
			}

			out.write("\n");
			out.write("</tbody></table>\n");

			out.write("");
			out.write("</body></html>");

			out.close();
		} catch (final IOException e) {
			e.printStackTrace();
		}
	}

}
