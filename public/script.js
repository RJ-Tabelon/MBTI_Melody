// Global variables to store MBTI and genre
let mbti = '';
let genre = '';

// Function to submit MBTI type
function submitMBTI() {
    mbti = document.getElementById('mbti-input').value.trim().toUpperCase();
    if (mbti) {
        const mbtiRegex = /^(ISTJ|ISFJ|INFJ|INTJ|ISTP|ISFP|INFP|INTP|ESTP|ESFP|ENFP|ENTP|ESTJ|ESFJ|ENFJ|ENTJ)$/;
        if (mbtiRegex.test(mbti)) {
            // Store MBTI in sessionStorage before redirecting
            sessionStorage.setItem('mbti', mbti);
            window.location.href = 'genre.html';
        } else {
            alert('Please enter a valid MBTI type (e.g., INFP, ENTJ)');
        }
    } else {
        alert('Please enter your MBTI type.');
    }
}

// Function to submit Genre
async function submitGenre() {
    // Retrieve MBTI from sessionStorage
    mbti = sessionStorage.getItem('mbti') || '';
    
    genre = document.getElementById('genre-input').value.trim();
    
    if (mbti && genre) {
        try {
            const response = await fetch('/run-cpp', {
                method: 'POST',
                headers: { 
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({ mbti, genre })
            });

            if (!response.ok) {
                const errorText = await response.text();
                throw new Error(`HTTP error! status: ${response.status}, message: ${errorText}`);
            }

            const data = await response.json();
            
            // More flexible recommendation extraction
            const recommendation = 
                data.output || 
                data.recommendation || 
                data.result || 
                JSON.stringify(data) || 
                'No recommendation available';

            // Ensure recommendation is stored
            sessionStorage.setItem('cppOutput', recommendation);
            window.location.href = 'results.html';
        } catch (error) {
            console.error('Fetch error:', error);
            alert(`Failed to get recommendation: ${error.message}. Please try again.`);
        }
    } else {
        alert('Please ensure both MBTI and genre are entered.');
    }
}

// Function to display recommendation on results page
function displayRecommendation() {
    const recommendation = sessionStorage.getItem('cppOutput') || 'No recommendation available.';
    const recommendationElement = document.getElementById('recommendation');
    
    if (recommendationElement) {
        try {
            // More robust recommendation parsing
            let formattedRecommendation;
            
            // Try to parse as JSON if it looks like a JSON string
            if (recommendation.trim().startsWith('{') || recommendation.trim().startsWith('[')) {
                try {
                    const parsedRec = JSON.parse(recommendation);
                    formattedRecommendation = `<p>${JSON.stringify(parsedRec, null, 2)}</p>`;
                } catch {
                    // If JSON parsing fails, fallback to string formatting
                    formattedRecommendation = recommendation
                        .split('\n')
                        .map(line => `<p>${line.trim()}</p>`)
                        .join('');
                }
            } else {
                // Regular string formatting
                formattedRecommendation = recommendation
                    .split('\n')
                    .map(line => `<p>${line.trim()}</p>`)
                    .join('');
            }
            
            recommendationElement.innerHTML = formattedRecommendation;
        } catch (error) {
            console.error('Recommendation display error:', error);
            recommendationElement.innerHTML = `<p>Error displaying recommendation: ${error.message}</p>`;
        }
    } else {
        console.error('Recommendation element not found');
    }
}