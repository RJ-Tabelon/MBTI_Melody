const express = require('express');
const bodyParser = require('body-parser');
const { exec } = require('child_process');
const path = require('path');
const fs = require('fs');

const app = express();
const PORT = process.env.PORT || 3000;

// CORS Middleware
app.use((req, res, next) => {
  res.header('Access-Control-Allow-Origin', '*');
  res.header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
  res.header('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept');
  next();
});

// Middleware
app.use(bodyParser.json());
app.use(express.static('public')); // Serve static files from public directory

// API Endpoint to Execute C++ Code
app.post('/run-cpp', (req, res) => {
  const { mbti, genre } = req.body;

  // Validate inputs
  if (!mbti || !genre) {
    return res.status(400).json({ error: 'MBTI and genre are required' });
  }

  console.log('Received inputs:', { mbti, genre }); // Debug log

  // Construct command to execute the C++ program
 // Construct command to execute the C++ program
const cppFilePath = path.join(__dirname, 'backend', 'main.cpp');
const outputFilePath = path.join(__dirname, 'backend', 'main');

  // Ensure the C++ file exists
  if (!fs.existsSync(cppFilePath)) {
    console.error(`C++ file not found: ${cppFilePath}`);
    return res.status(500).json({ error: 'C++ source file not found' });
  }

  // Compilation command
  const compileCommand = `g++ ${cppFilePath} -o ${outputFilePath}`;
  
  // Execute compilation
  exec(compileCommand, (compileError, compileStdout, compileStderr) => {
    if (compileError || compileStderr) {
      console.error(`Compilation Error: ${compileError || compileStderr}`);
      return res.status(500).json({ error: 'Failed to compile C++ program', details: compileError || compileStderr });
    }

    // Execution command with input
    const runCommand = `echo "${mbti} ${genre}" | ${outputFilePath}`;

    console.log('Execution command:', runCommand); // Debug log

    // Execute the compiled program
    exec(runCommand, (runError, stdout, stderr) => {
      // Remove the compiled executable
      try {
        fs.unlinkSync(outputFilePath);
      } catch (unlinkError) {
        console.error('Error removing executable:', unlinkError);
      }

      if (runError || stderr) {
        console.error(`Execution Error: ${runError || stderr}`);
        return res.status(500).json({ 
          error: 'Error executing the C++ program', 
          details: runError || stderr 
        });
      }

      // Send the output back to the client
      console.log('Program output:', stdout.trim()); // Debug log
      res.json({ output: stdout.trim() });
    });
  });
});

// Catch-all route to serve index.html for client-side routing
app.get('*', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Start the server
app.listen(PORT, () => {
  console.log(`Server is running at http://localhost:${PORT}`);
});