package com.example.homelights;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;


public class HomeLights extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
    }

	/** Called when the user clicks the Send button */
	public void sendMessage(View view) {
		// Do something in response to button
		runUdpClient();
	}

	private static final int UDP_SERVER_PORT = 32000;

	private void runUdpClient()  {
		String udpMsg = "toggle\n";
		DatagramSocket ds = null;
		try {
				ds = new DatagramSocket();
				InetAddress serverAddr = InetAddress.getByName("192.168.88.129");
				DatagramPacket dp;
				dp = new DatagramPacket(udpMsg.getBytes(), udpMsg.length(), serverAddr, UDP_SERVER_PORT);
				ds.send(dp);
			} catch (SocketException e) {
				e.printStackTrace();
			}catch (UnknownHostException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (ds != null) {
					ds.close();
				}
			}
	} // close run UdpClient
} // close Activity

